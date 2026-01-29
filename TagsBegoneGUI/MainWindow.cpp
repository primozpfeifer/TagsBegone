#pragma once

#include "MainWindow.hpp"


MainWindow::MainWindow()
{
    createMenus();
    createWidgets();

    setWindowTitle("Tags Begone");
    setFixedSize(500, 265);

    statusBar()->showMessage("No source specified", 0);
    statusBar()->setSizeGripEnabled(false);
}


void MainWindow::createMenus()
{
    // menu "File"
    QMenu* menu_file = menuBar()->addMenu("&File");

    QAction* act_selectFile = new QAction("&Select file", this);
    menu_file->addAction(act_selectFile);
    connect(act_selectFile, &QAction::triggered, this, &MainWindow::selectFile);

    QAction* act_selectDir = new QAction("&Select directory", this);
    menu_file->addAction(act_selectDir);
    connect(act_selectDir, &QAction::triggered, this, &MainWindow::selectDir);

    menu_file->addSeparator();

    QAction* act_exit = new QAction("E&xit", this);
    menu_file->addAction(act_exit);
    connect(act_exit, &QAction::triggered, this, &QWidget::close);

    // menu "About"
    QMenu* menu_about = menuBar()->addMenu("&About");

    QAction* act_about = new QAction("&About", this);
    menu_about->addAction(act_about);
    connect(act_about, &QAction::triggered, this, &MainWindow::selectFile);
}

void MainWindow::createWidgets()
{
    QWidget* centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    // groupbox "Source directory"
    QGroupBox* gbSource = new QGroupBox("Source directory", this);
    lineEdit_source = new QLineEdit();
    lineEdit_source->setFixedHeight(30);
    connect(lineEdit_source, &QLineEdit::textChanged, this, &MainWindow::updateRemoveTagsButton);
    QToolButton* pbDirectory = new QToolButton(this);
    pbDirectory->setText("...");
    pbDirectory->setFixedSize(30, 30);
    connect(pbDirectory, &QToolButton::clicked, this, &MainWindow::selectDir);
    checkBox_inclSubdirs = new QCheckBox(" Include subdirectories", this);
    QVBoxLayout* vbox1 = new QVBoxLayout;
    QHBoxLayout* hbox1 = new QHBoxLayout;
    QHBoxLayout* hbox2 = new QHBoxLayout;
    vbox1->setContentsMargins(10, 0, 10, 5);
    hbox2->setContentsMargins(10, 0, 0, 0);
    hbox1->addWidget(lineEdit_source);
    hbox1->addWidget(pbDirectory);
    vbox1->addLayout(hbox1);
    hbox2->addWidget(checkBox_inclSubdirs);
    vbox1->addLayout(hbox2);
    gbSource->setLayout(vbox1);
    gbSource->setFixedHeight(100);

    // groupbox "Remove Tags"
    QGroupBox* gbTags = new QGroupBox("Remove tags", this);
    checkBox_id3v1 = new QCheckBox(" ID3v1", this);
    checkBox_id3v1->setChecked(true);
    connect(checkBox_id3v1, &QCheckBox::checkStateChanged, this, &MainWindow::updateRemoveTagsButton);
    checkBox_id3v2 = new QCheckBox(" ID3v2", this);
    checkBox_id3v2->setChecked(true);
    connect(checkBox_id3v2, &QCheckBox::checkStateChanged, this, &MainWindow::updateRemoveTagsButton);
    checkBox_apev2 = new QCheckBox(" APEv2", this);
    checkBox_apev2->setChecked(true);
    connect(checkBox_apev2, &QCheckBox::checkStateChanged, this, &MainWindow::updateRemoveTagsButton);
    button_removeTags = new QPushButton("START", this);
    button_removeTags->setFixedSize(100, 40);
    button_removeTags->setEnabled(false);
    connect(button_removeTags, &QPushButton::clicked, this, &MainWindow::removeTags);
    QHBoxLayout* hbox3 = new QHBoxLayout;
    hbox3->setSpacing(30);
    hbox3->setContentsMargins(10, 0, 20, 5);
    hbox3->addWidget(button_removeTags);
    hbox3->addWidget(checkBox_id3v1);
    hbox3->addWidget(checkBox_id3v2);
    hbox3->addWidget(checkBox_apev2);
    gbTags->setLayout(hbox3);
    gbTags->setFixedHeight(75);

    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(gbSource);
    mainLayout->addWidget(gbTags);
    centralWidget->setLayout(mainLayout);
}


void MainWindow::selectFile()
{
    QFileDialog dialog(this);
    QString qstrPath = dialog.getOpenFileName(this, "Select a file", "", "Mp3 audio files (*.mp3)");

    if (!qstrPath.isEmpty())
    {
        lineEdit_source->setText(qstrPath);
    }
}

void MainWindow::selectDir()
{
    QString qstrPath = QFileDialog::getExistingDirectory(this, "Select a directory");

    if (!qstrPath.isEmpty())
    {
        lineEdit_source->setText(qstrPath);
    }
}

void MainWindow::removeTags()
{
    statusBar()->showMessage("Scanning files and removing tags...");

    std::filesystem::path sourcePath(lineEdit_source->text().toStdWString());

    if (std::filesystem::exists(sourcePath) &&
        std::filesystem::is_directory(sourcePath))
    {
        RemoveTags::process_directory(statusBar(), sourcePath, checkBox_inclSubdirs->isChecked(), checkBox_id3v1->isChecked(), checkBox_id3v2->isChecked(), checkBox_apev2->isChecked());
    }

    else if (std::filesystem::exists(sourcePath) &&
        std::filesystem::is_regular_file(sourcePath))
    {
        RemoveTags::process_file(statusBar(), sourcePath, checkBox_id3v1->isChecked(), checkBox_id3v2->isChecked(), checkBox_apev2->isChecked());
    }
    else
    {
        statusBar()->showMessage("Source not found or incorrect");
    }
}

void MainWindow::updateRemoveTagsButton()
{
    if (!lineEdit_source->text().isEmpty() &&
        (checkBox_id3v1->isChecked() ||
        checkBox_id3v2->isChecked() ||
        checkBox_apev2->isChecked()))
    {
        button_removeTags->setEnabled(true);
    }
    else
    {
        button_removeTags->setEnabled(false);
    }
}
