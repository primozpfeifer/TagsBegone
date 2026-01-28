#pragma once

#include <QMainWindow>
#include <QtWidgets>

#include <filesystem>
#include "RemoveTags.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow();


private:

    void createMenus();
    void createWidgets();

    QLineEdit* lineEdit_source;
    QCheckBox* checkBox_subdirs;
    QPushButton* button_removeTags;
    QCheckBox* checkBox_id3v1;
    QCheckBox* checkBox_id3v2;
    QCheckBox* checkBox_apev2;


private slots:

    void openFile();
    void openFolder();
    void removeTags();
};

