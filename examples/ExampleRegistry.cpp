#include "examples/ExampleRegistry.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

ExampleRegistry& ExampleRegistry::instance()
{
    static ExampleRegistry g_instance;
    return g_instance;
}

void ExampleRegistry::registerFactory(const QString& className, Factory factory)
{
    m_factories.insert(className, std::move(factory));
}

bool ExampleRegistry::loadManifest(const QString& manifestPath, QString* errorString)
{
    QFile file(manifestPath);
    if (!file.open(QIODevice::ReadOnly)) {
        if (errorString) {
            *errorString = QStringLiteral("Unable to open manifest %1").arg(manifestPath);
        }
        return false;
    }

    const auto jsonData = file.readAll();
    QJsonParseError parseError;
    const auto doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (doc.isNull()) {
        if (errorString) {
            *errorString = QStringLiteral("Manifest parse error at offset %1: %2")
                               .arg(parseError.offset)
                               .arg(parseError.errorString());
        }
        return false;
    }

    m_categories.clear();
    m_categoryMap.clear();
    m_definitions.clear();

    const auto rootObj = doc.object();
    const auto categories = rootObj.value(QStringLiteral("categories")).toArray();
    for (const auto& value : categories) {
        if (!value.isObject()) {
            continue;
        }
        const auto obj = value.toObject();
        ExampleCategoryInfo info;
        info.id = obj.value(QStringLiteral("id")).toString();
        info.title = obj.value(QStringLiteral("title")).toString(info.id);
        info.description = obj.value(QStringLiteral("description")).toString();
        info.iconPath = obj.value(QStringLiteral("icon")).toString();
        if (!info.id.isEmpty()) {
            m_categoryMap.insert(info.id, {});
            m_categories.append(info);
        }
    }

    const auto examples = rootObj.value(QStringLiteral("examples")).toArray();
    for (const auto& value : examples) {
        if (!value.isObject()) {
            continue;
        }
        const auto obj = value.toObject();
        const auto exampleId = obj.value(QStringLiteral("id")).toString();
        const auto className = obj.value(QStringLiteral("class")).toString();

        if (exampleId.isEmpty() || className.isEmpty()) {
            if (errorString) {
                *errorString = QStringLiteral("Example entry missing id or class.");
            }
            return false;
        }

        auto maybeDefinition = buildDefinition(className, exampleId, obj, errorString);
        if (!maybeDefinition.has_value()) {
            return false;
        }

        ExampleDefinition definition = maybeDefinition.value();
        const QString categoryId = definition.metadata.categoryId;

        if (!m_categoryMap.contains(categoryId) && !categoryId.isEmpty()) {
            ExampleCategoryInfo synthetic;
            synthetic.id = categoryId;
            synthetic.title = categoryId;
            m_categories.append(synthetic);
            m_categoryMap.insert(categoryId, {});
        }

        m_definitions.insert(exampleId, definition);
        if (!categoryId.isEmpty()) {
            m_categoryMap[categoryId].append(exampleId);
        }
    }

    return true;
}

QList<ExampleCategoryInfo> ExampleRegistry::categories() const
{
    return m_categories;
}

QStringList ExampleRegistry::exampleIdsForCategory(const QString& categoryId) const
{
    return m_categoryMap.value(categoryId);
}

ExampleDefinition ExampleRegistry::definitionForId(const QString& exampleId) const
{
    return m_definitions.value(exampleId);
}

QList<ExampleDefinition> ExampleRegistry::definitions() const
{
    return m_definitions.values();
}

ExamplePtr ExampleRegistry::createExample(const QString& exampleId) const
{
    const auto definition = m_definitions.value(exampleId);
    if (definition.className.isEmpty()) {
        return nullptr;
    }

    const auto factory = m_factories.value(definition.className);
    if (!factory) {
        return nullptr;
    }
    return factory();
}

std::optional<ExampleDefinition> ExampleRegistry::buildDefinition(const QString& className,
                                                                  const QString& exampleId,
                                                                  const QJsonObject& overrides,
                                                                  QString* errorString) const
{
    const auto factory = m_factories.value(className);
    if (!factory) {
        if (errorString) {
            *errorString = QStringLiteral("Factory not registered for class %1").arg(className);
        }
        return std::nullopt;
    }

    ExamplePtr instance = factory();
    if (!instance) {
        if (errorString) {
            *errorString = QStringLiteral("Unable to instantiate example %1").arg(className);
        }
        return std::nullopt;
    }

    ExampleDefinition definition;
    definition.id = exampleId;
    definition.className = className;
    ExampleMetadata metadata = instance->metadata();
    metadata.id = exampleId;

    if (overrides.contains(QStringLiteral("title"))) {
        metadata.title = overrides.value(QStringLiteral("title")).toString(metadata.title);
    }
    if (overrides.contains(QStringLiteral("description"))) {
        metadata.summary = overrides.value(QStringLiteral("description")).toString(metadata.summary);
    }
    if (overrides.contains(QStringLiteral("category"))) {
        metadata.categoryId = overrides.value(QStringLiteral("category")).toString(metadata.categoryId);
    }
    if (overrides.contains(QStringLiteral("tags")) && overrides.value(QStringLiteral("tags")).isArray()) {
        QStringList tags;
        const auto tagArray = overrides.value(QStringLiteral("tags")).toArray();
        for (const auto& tagValue : tagArray) {
            tags.append(tagValue.toString());
        }
        metadata.tags = tags;
    }
    if (overrides.contains(QStringLiteral("dataPath"))) {
        metadata.dataPath = overrides.value(QStringLiteral("dataPath")).toString(metadata.dataPath);
    }
    if (overrides.contains(QStringLiteral("thumbnail"))) {
        metadata.thumbnailPath = overrides.value(QStringLiteral("thumbnail")).toString(metadata.thumbnailPath);
    }
    if (overrides.contains(QStringLiteral("documentation"))) {
        metadata.documentationLink = overrides.value(QStringLiteral("documentation")).toString(metadata.documentationLink);
    }

    definition.metadata = metadata;
    definition.parameters = instance->parameters();
    return definition;
}

ExampleRegistrar::ExampleRegistrar(const QString& className, ExampleRegistry::Factory factory)
{
    ExampleRegistry::instance().registerFactory(className, std::move(factory));
}
