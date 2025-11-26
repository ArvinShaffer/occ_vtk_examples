#ifndef EXAMPLEREGISTRY_H
#define EXAMPLEREGISTRY_H

#include "examples/Example.h"

#include <QObject>
#include <QHash>
#include <QVector>
#include <QJsonObject>
#include <functional>
#include <optional>

struct ExampleCategoryInfo {
    QString id;
    QString title;
    QString description;
    QString iconPath;
};

struct ExampleDefinition {
    QString id;
    QString className;
    ExampleMetadata metadata;
    QList<ExampleParameter> parameters;
};

class ExampleRegistry : public QObject
{
    Q_OBJECT
public:
    using Factory = std::function<ExamplePtr()>;

    static ExampleRegistry& instance();

    void registerFactory(const QString& className, Factory factory);
    bool loadManifest(const QString& manifestPath, QString* errorString);

    QList<ExampleCategoryInfo> categories() const;
    QStringList exampleIdsForCategory(const QString& categoryId) const;

    ExampleDefinition definitionForId(const QString& exampleId) const;
    QList<ExampleDefinition> definitions() const;
    ExamplePtr createExample(const QString& exampleId) const;

private:
    ExampleRegistry() = default;

    std::optional<ExampleDefinition> buildDefinition(const QString& className,
                                                     const QString& exampleId,
                                                     const QJsonObject& overrides,
                                                     QString* errorString) const;

    mutable QHash<QString, ExampleDefinition> m_definitions;
    QHash<QString, QStringList> m_categoryMap;
    QList<ExampleCategoryInfo> m_categories;
    QHash<QString, Factory> m_factories;
};

class ExampleRegistrar
{
public:
    ExampleRegistrar(const QString& className, ExampleRegistry::Factory factory);
};

#endif // EXAMPLEREGISTRY_H
