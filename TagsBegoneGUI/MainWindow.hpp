#pragma once

#include <QMainWindow>
#include <QtWidgets>


class MainWindow : public QMainWindow
{
public:
    MainWindow();


private slots:
    void openFiles();
    void openFolder();


private:
    void createWidgets();
    void createActions();
    void createMenus();

    //QMenuBar* menuBar;
    QMenu* fileMenu;
    QAction* openFilesAct;
    QAction* openFolderAct;
    QAction* exitAct;
    QLineEdit* lineEdit;
};

