#pragma once

#include "MainWindow.hpp"


MainWindow::MainWindow()
{
    setWindowTitle(tr("Tags Begone"));
    setMinimumSize(160, 160);
    resize(480, 320);
    

    createWidgets();
    createActions();
    createMenus();

}


void MainWindow::openFiles()
{
    //QString path = QFileDialog::getOpenFileName(this, "Choose a file or folder...");
    
    QFileDialog dialog(this);
    QString path = dialog.getOpenFileName(this, tr("Select file(s)"), "", tr("Mp3 audio files (*.mp3)"));

    if (!path.isEmpty()) {
        QFile file(path);
        

        lineEdit->setText(path);
    }

}

void MainWindow::openFolder()
{
    QFileDialog dialog(this);
    QString path = dialog.getExistingDirectory();


}


void MainWindow::createWidgets()
{
    QWidget* topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* widget = new QWidget;
    setCentralWidget(widget);
    lineEdit = new QLineEdit("No file or folder specified!");
    lineEdit->setReadOnly(true);

    QWidget* bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(topFiller);
    layout->addWidget(lineEdit);
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
