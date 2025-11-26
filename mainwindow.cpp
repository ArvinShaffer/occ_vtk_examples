#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QSplitter>
#include <QStringList>
#include <QTextBrowser>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <utility>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_registry(ExampleRegistry::instance())
{
    ui->setupUi(this);

    m_dataRoot = locateDataDirectory();
    const QString manifestPath = QDir(m_dataRoot).filePath(QStringLiteral("examples_manifest.json"));
    if (!QFileInfo::exists(manifestPath)) {
        appendLog(tr("警告: 在 %1 未找到示例清单。").arg(manifestPath));
    }

    initializeRenderWindow();
    initializeNavigation();
    loadManifest();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeRenderWindow()
{
    renWin3d = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    main_render_3d = vtkSmartPointer<vtkRenderer>::New();
    colors = vtkSmartPointer<vtkNamedColors>::New();
    main_render_3d->SetBackground(colors->GetColor3d("SteelBlue").GetData());

    renWin3d->AddRenderer(main_render_3d);
    ui->openGLWidget->setRenderWindow(renWin3d);
}

void MainWindow::initializeNavigation()
{
    ui->listExamples->setResizeMode(QListView::Adjust);
    ui->listExamples->setViewMode(QListView::IconMode);
    ui->listExamples->setSpacing(8);
    ui->listExamples->setIconSize(QSize(48, 48));
    ui->listExamples->setSelectionMode(QAbstractItemView::SingleSelection);

    if (!ui->paramsFormContainer->layout()) {
        m_parametersLayout = new QFormLayout(ui->paramsFormContainer);
    } else {
        m_parametersLayout = qobject_cast<QFormLayout*>(ui->paramsFormContainer->layout());
    }

    m_parametersLayout->setSpacing(12);
    m_parametersLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    connect(ui->treeCategories, &QTreeWidget::itemSelectionChanged, this, &MainWindow::handleCategorySelection);
    connect(ui->listExamples, &QListWidget::itemSelectionChanged, this, &MainWindow::handleExampleSelection);
    connect(ui->listExamples, &QListWidget::itemDoubleClicked, this, &MainWindow::handleExampleActivation);
    connect(ui->btnRunExample, &QPushButton::clicked, this, &MainWindow::handleRunExample);
    connect(ui->lineSearch, &QLineEdit::textChanged, this, &MainWindow::handleSearchTextChanged);
}

void MainWindow::loadManifest()
{
    QString error;
    const QString manifestPath = resolveDataPath(QStringLiteral("examples_manifest.json"));
    if (!m_registry.loadManifest(manifestPath, &error)) {
        QMessageBox::warning(this, tr("Manifest Error"), error);
        return;
    }
    populateCategoryTree();
}

void MainWindow::populateCategoryTree()
{
    ui->treeCategories->clear();
    const auto categories = m_registry.categories();
    for (const auto& category : categories) {
        auto* item = new QTreeWidgetItem(ui->treeCategories);
        item->setText(0, category.title);
        item->setToolTip(0, category.description);
        item->setData(0, Qt::UserRole, category.id);
    }
    ui->treeCategories->expandAll();
    if (ui->treeCategories->topLevelItemCount() > 0) {
        ui->treeCategories->setCurrentItem(ui->treeCategories->topLevelItem(0));
    }
}

void MainWindow::populateExamplesForCategory(const QString& categoryId)
{
    ui->listExamples->clear();
    QStringList exampleIds;
    if (categoryId.isEmpty()) {
        const auto definitionList = m_registry.definitions();
        for (const auto& definition : definitionList) {
            exampleIds.append(definition.id);
        }
    } else {
        exampleIds = m_registry.exampleIdsForCategory(categoryId);
    }

    for (const auto& exampleId : exampleIds) {
        const auto definition = m_registry.definitionForId(exampleId);
        if (definition.id.isEmpty()) {
            continue;
        }
        auto* item = new QListWidgetItem(definition.metadata.title, ui->listExamples);
        item->setToolTip(definition.metadata.summary);
        item->setData(Qt::UserRole, definition.id);
        item->setData(Qt::UserRole + 1, definition.metadata.summary);
    }
    applySearchFilter(ui->lineSearch->text());
}

void MainWindow::applySearchFilter(const QString& pattern)
{
    const QString needle = pattern.trimmed();
    for (int i = 0; i < ui->listExamples->count(); ++i) {
        auto* item = ui->listExamples->item(i);
        const bool matches = needle.isEmpty()
            || item->text().contains(needle, Qt::CaseInsensitive)
            || item->data(Qt::UserRole + 1).toString().contains(needle, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

void MainWindow::displayExample(const QString& exampleId)
{
    m_currentExampleId = exampleId;
    m_currentDefinition = m_registry.definitionForId(exampleId);
    if (m_currentDefinition.id.isEmpty()) {
        appendLog(tr("找不到示例 %1 的定义。").arg(exampleId));
        return;
    }
    ui->lblExampleTitle->setText(m_currentDefinition.metadata.title);
    ui->txtDescription->setText(m_currentDefinition.metadata.summary);
    rebuildParameterPanel();
}

void MainWindow::rebuildParameterPanel()
{
    if (!m_parametersLayout) {
        return;
    }

    while (auto* item = m_parametersLayout->takeAt(0)) {
        if (auto* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }

    m_parameterEditors.clear();
    m_currentParameters = m_currentDefinition.parameters;

    for (const auto& parameter : m_currentParameters) {
        QWidget* editor = createEditor(parameter);
        if (!editor) {
            continue;
        }
        if (!parameter.description.isEmpty()) {
            editor->setToolTip(parameter.description);
        }
        m_parameterEditors.insert(parameter.id, editor);
        m_parametersLayout->addRow(parameter.label, editor);
    }

    if (m_parameterEditors.isEmpty()) {
        auto* hintLabel = new QLabel(tr("此示例没有可调节参数。"), ui->paramsFormContainer);
        hintLabel->setObjectName(QStringLiteral("paramsHint"));
        hintLabel->setWordWrap(true);
        m_parametersLayout->addRow(QString(), hintLabel);
    }
}

QWidget* MainWindow::createEditor(const ExampleParameter& parameter)
{
    switch (parameter.type) {
    case ExampleParameterType::Boolean: {
        auto* checkbox = new QCheckBox(ui->paramsFormContainer);
        checkbox->setChecked(parameter.defaultValue.toBool());
        return checkbox;
    }
    case ExampleParameterType::Integer: {
        auto* spin = new QSpinBox(ui->paramsFormContainer);
        if (parameter.minimumValue.isValid()) {
            spin->setMinimum(parameter.minimumValue.toInt());
        }
        if (parameter.maximumValue.isValid()) {
            spin->setMaximum(parameter.maximumValue.toInt());
        }
        spin->setValue(parameter.defaultValue.toInt());
        spin->setSingleStep(static_cast<int>(parameter.singleStep));
        return spin;
    }
    case ExampleParameterType::Double: {
        auto* spin = new QDoubleSpinBox(ui->paramsFormContainer);
        if (parameter.minimumValue.isValid()) {
            spin->setMinimum(parameter.minimumValue.toDouble());
        }
        if (parameter.maximumValue.isValid()) {
            spin->setMaximum(parameter.maximumValue.toDouble());
        }
        spin->setDecimals(3);
        spin->setValue(parameter.defaultValue.toDouble());
        spin->setSingleStep(parameter.singleStep);
        return spin;
    }
    case ExampleParameterType::Choice: {
        auto* combo = new QComboBox(ui->paramsFormContainer);
        combo->addItems(parameter.options);
        const int index = combo->findText(parameter.defaultValue.toString());
        if (index >= 0) {
            combo->setCurrentIndex(index);
        }
        return combo;
    }
    case ExampleParameterType::FilePath: {
        auto* container = new QWidget(ui->paramsFormContainer);
        auto* layout = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(4);

        auto* lineEdit = new QLineEdit(container);
        lineEdit->setText(parameter.defaultValue.toString());
        lineEdit->setPlaceholderText(parameter.placeholderText);
        layout->addWidget(lineEdit);

        auto* button = new QToolButton(container);
        button->setText("...");
        layout->addWidget(button);

        connect(button, &QToolButton::clicked, this, [this, lineEdit]() {
            const QString filePath = QFileDialog::getOpenFileName(this, tr("选择 VTK 文件"), m_dataRoot, tr("VTK (*.vtk)"));
            if (!filePath.isEmpty()) {
                lineEdit->setText(filePath);
            }
        });
        return container;
    }
    }
    return nullptr;
}

QVariant MainWindow::editorValue(const ExampleParameter& parameter, QWidget* editor) const
{
    if (!editor) {
        return parameter.defaultValue;
    }
    switch (parameter.type) {
    case ExampleParameterType::Boolean:
        return qobject_cast<QCheckBox*>(editor)->isChecked();
    case ExampleParameterType::Integer:
        return qobject_cast<QSpinBox*>(editor)->value();
    case ExampleParameterType::Double:
        return qobject_cast<QDoubleSpinBox*>(editor)->value();
    case ExampleParameterType::Choice:
        return qobject_cast<QComboBox*>(editor)->currentText();
    case ExampleParameterType::FilePath: {
        auto* lineEdit = editor->findChild<QLineEdit*>();
        return lineEdit ? lineEdit->text() : parameter.defaultValue;
    }
    }
    return parameter.defaultValue;
}

QVariantMap MainWindow::gatherParameters() const
{
    QVariantMap values;
    for (const auto& parameter : m_currentParameters) {
        auto* editor = m_parameterEditors.value(parameter.id, nullptr);
        values.insert(parameter.id, editorValue(parameter, editor));
    }
    return values;
}

QString MainWindow::resolveDataPath(const QString& relative) const
{
    if (relative.isEmpty()) {
        return {};
    }
    QFileInfo info(relative);
    if (info.isAbsolute() || relative.startsWith(":/")) {
        return relative;
    }
    return QDir(m_dataRoot).filePath(relative);
}

QString MainWindow::locateDataDirectory() const
{
    const QString exeDir = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        QDir(exeDir).filePath("data"),
        QDir(exeDir).filePath("../data"),
        QDir(exeDir).filePath("../../data"),
        QDir(exeDir).filePath("../../../data"),
        QDir(exeDir).filePath("../share/occvtkex/data"),
        QDir(exeDir).filePath("../../share/occvtkex/data")
    };

    for (const QString& candidate : candidates) {
        const QString manifest = QDir(candidate).filePath("examples_manifest.json");
        if (QFileInfo::exists(manifest)) {
            return candidate;
        }
    }
    // Fallback to executable directory if nothing matches
    return exeDir;
}

void MainWindow::runActiveExample()
{
    if (m_currentExampleId.isEmpty()) {
        appendLog(tr("请先选择一个示例。"));
        return;
    }

    ExamplePtr instance = m_registry.createExample(m_currentExampleId);
    if (!instance) {
        appendLog(tr("无法创建示例 %1").arg(m_currentExampleId));
        return;
    }

    ExampleContext context(main_render_3d, renWin3d, ui->openGLWidget, m_dataRoot);
    const QVariantMap params = gatherParameters();
    instance->run(params, context);
    m_activeExample = std::move(instance);
    if (renWin3d) {
        renWin3d->Render();
    }
    if (ui->openGLWidget) {
        ui->openGLWidget->update();
    }
    appendLog(tr("%1 已执行。").arg(m_currentDefinition.metadata.title));
}

void MainWindow::appendLog(const QString& message)
{
    ui->txtLog->appendPlainText(
        QStringLiteral("[%1] %2").arg(QDateTime::currentDateTime().toString("HH:mm:ss"), message));
}

void MainWindow::handleCategorySelection()
{
    const auto selectedItems = ui->treeCategories->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }
    const QString categoryId = selectedItems.constFirst()->data(0, Qt::UserRole).toString();
    m_currentCategoryId = categoryId;
    populateExamplesForCategory(categoryId);
    if (ui->listExamples->count() > 0) {
        ui->listExamples->setCurrentRow(0);
    }
}

void MainWindow::handleExampleSelection()
{
    const auto selectedItems = ui->listExamples->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }
    const QString exampleId = selectedItems.constFirst()->data(Qt::UserRole).toString();
    displayExample(exampleId);
}

void MainWindow::handleExampleActivation(QListWidgetItem* item)
{
    if (!item) {
        return;
    }
    displayExample(item->data(Qt::UserRole).toString());
    runActiveExample();
}

void MainWindow::handleRunExample()
{
    runActiveExample();
}

void MainWindow::handleSearchTextChanged(const QString& text)
{
    applySearchFilter(text);
}
