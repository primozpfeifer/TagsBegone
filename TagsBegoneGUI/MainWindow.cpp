#pragma once

#include "MainWindow.hpp"


MainWindow::MainWindow()
{
    setWindowTitle(tr("Tags Begone"));
    setMinimumSize(160, 160);
    resize(450, 180);
    

    createWidgets();
    createActions();
    createMenus();

}


void MainWindow::openFiles()
{
    //QString path = QFileDialog::getOpenFileName(this, "Choose a file or folder...");
    
    /*QFileDialog dialog(this);
    QString path = dialog.getOpenFileName(this, tr("Select file(s)"), "", tr("Mp3 audio files (*.mp3)"));

    if (!path.isEmpty()) {
        //QFile file(path);
        

        lineEdit->setText(path);
    }
    */
}

void MainWindow::openFolder()
{
    QString qStrPath = QFileDialog::getExistingDirectory(this, tr("Select a folder"));

    if (!qStrPath.isEmpty())
    {
        lineEdit->setText(qStrPath);
        
        /*
        std::filesystem::path input_path(qStrPath.toStdWString());
        RemoveTags remTags;
        remTags.processFolder(*lineEdit, input_path);
        */
    }
    else
    {
        lineEdit->setText(tr("No file or folder specified!"));
    }
}


void MainWindow::createWidgets()
{
    QWidget* topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    lineEdit = new QLineEdit("No file or folder specified!");
    lineEdit->setReadOnly(true);

    QHBoxLayout* buttons = new QHBoxLayout;
    QCheckBox* cb1 = new QCheckBox(tr("ID3v1"), this);
    buttons->addWidget(cb1);
    QCheckBox* cb2 = new QCheckBox(tr("ID3v2"), this);
    buttons->addWidget(cb2);
    QCheckBox* cb3 = new QCheckBox(tr("APEv2"), this);
    buttons->addWidget(cb3);
    pbRemoveTags = new QPushButton(tr("Remove tags"), this);
    pbRemoveTags->setFixedSize(100, 40);
    buttons->addWidget(pbRemoveTags);
    QPushButton* pbClearSource = new QPushButton(tr("Clear source"), this);
    pbClearSource->setFixedSize(100, 40);
    buttons->addWidget(pbClearSource);


    QWidget* bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(topFiller);
    layout->addWidget(lineEdit);
    layout->addLayout(buttons);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);
}

void MainWindow::createActions()
{
    openFilesAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), tr("&Open file(s)"), this);
    openFilesAct->setStatusTip(tr("Add file(s)"));
    connect(openFilesAct, &QAction::triggered, this, &MainWindow::openFiles);

    openFolderAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen), tr("&Open folder"), this);
    openFolderAct->setStatusTip(tr("Add a folder"));
    connect(openFolderAct, &QAction::triggered, this, &MainWindow::openFolder);

    exitAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit), tr("E&xit"), this);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::createMenus()
{
    //menuBar = new QMenuBar(this);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFilesAct);
    fileMenu->addAction(openFolderAct);

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}
