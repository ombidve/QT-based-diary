//QT MODULE
#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<fstream>
#include "database.h"
#include <QMessageBox>
#include "login.h"
#include<QMenuBar>
#include<QMenu>
#include "encryptDecrypt.h"
#include<QDate>
#include "clearSpace.h"
#include "qfilesystemmodeldialog.h"
#include<filesystem>
#include <unistd.h>
#include "spass.h"

using std::cout; using std::cin;
using std::endl; using std::string;
using std::filesystem::current_path;


using std::cout; using std::endl;
using std::system; using std::string;
namespace fs = std::filesystem;

bool LOGIN = false;
bool HIDDEN = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->frame->setDisabled(true);
    ui->widget->setVisible(false);   
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    Database registerUser;
            QMessageBox msgBox;
    //movie.insert_data("Movie name");
    QString temp;
    temp = ui->lineEdit_3->text();
    QString temp2 = ui->lineEdit_4->text();
    QString temp3 = ui->lineEdit_5->text();
    QString temp4 = ui->lineEdit_6->text();
    if(temp == "" || temp2 == "" || temp3 == "" || temp4 == "")
    {
        QMessageBox warning;
        warning.setText("Please enter a username and password!!");
        warning.exec();
    }
    else
    {
        if(ui->lineEdit_5->text() == ui->lineEdit_4->text() && strongPassword(ui->lineEdit_4->text().toStdString()))
        {
            temp = temp + "+" + ui->lineEdit_5->text();
            temp = temp + "+" + ui->lineEdit_6->text();
            string encryptedData = encryptDecrypt(temp.toStdString());
            registerUser.insert_data("users",encryptedData);
            ui->lineEdit_3->clear();
            ui->lineEdit_4->clear();
            ui->lineEdit_5->clear();
            ui->lineEdit_6->clear();
            msgBox.setText("Succesfully Saved User!!");
            msgBox.exec();
        }
        else
        {
            if(!strongPassword(ui->lineEdit_4->text().toStdString()))
            {
                msgBox.setText("Not strong password");
                msgBox.exec();
            }
            msgBox.setText("Password and Confirm Password Not same try again!!");
            msgBox.exec();
        }
    }
}


void MainWindow::on_pushButton_clicked()
{
    login user;
    QString username,password;
    username = ui->lineEdit->text();
    password = ui->lineEdit_2->text();
    int flag = user.loginUser(username.toStdString(),password.toStdString());
    if(flag == 1)
    {
        //login
        ui->tabWidget->close();
        ui->frame->setDisabled(false);
        ui->label_7->setText(username);
        LOGIN = true;
    }
    else
    {
        QMessageBox wrongunorpass;
        wrongunorpass.setText("Wrong Username or Password");
        wrongunorpass.exec();
    }
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void MainWindow::on_commandLinkButton_released()
{
    ui->label_7->clear();
    ui->frame->show();
    ui->frame->setDisabled(true);
    ui->tabWidget->show();
}



void MainWindow::on_pushButton_3_released()
{
    QString temp = ui->textEdit->toPlainText();
    ui->progressBar->setValue(10);
    if(temp == "")
    {
        QMessageBox empty;
        empty.setText("File will be Empty not saving!");
        empty.exec();
        ui->progressBar->reset();
        return;
    }
    QString uname = ui->label_7->text();
    Database userData(uname.toStdString());
    ui->progressBar->setValue(50);
    sleep(2);
    QString day = ui->dateEdit->date().toString("yyyy.MM.dd");
    ui->progressBar->setValue(75);
    //dandt.toStdString() = clearSpace(dandt.toStdString());
    //cout<<day;
    int flag = userData.insert_data(uname.toStdString(),day.toStdString(),temp);
    if(flag == 1)
    {
        cout<<"success";
    }
    ui->textEdit->clear();
    ui->progressBar->setValue(100);
    //userData.create_database(uname.toStdString(),2);
}



void MainWindow::on_pushButton_4_released()
{
    if(HIDDEN)
    {
    ui->frame->show();
    HIDDEN = false;
    }
    else
    {
        ui->frame->setVisible(false);
        HIDDEN = true;
    }
}




void MainWindow::on_pushButton_6_released()
{
    ui->textEdit->clear();
    ui->progressBar->reset();
    string uname  = ui->label_7->text().toStdString();
    string day = to_string(ui->dateEdit->date().day());
    string month = to_string(ui->dateEdit->date().month());
    string year = to_string(ui->dateEdit->date().year());
    QString fulldate = ui->dateEdit->date().toString("yyyy.MM.dd");
    string date = day + "" + month + "" + year;
    Database userData(uname);
    QString datedata = userData.show_data(uname,fulldate.toStdString());
    if(datedata == "")
    {
        QMessageBox notFound;
        notFound.setText("Couldn't Find required file!!");
        notFound.exec();
        return;
    }
    datedata.toStdString() = encryptDecrypt(datedata.toStdString());
    cout<<datedata.toStdString();
    for(auto i=0;i<101;i++)
    {
        ui->progressBar->setValue(i);
    }
    ui->textEdit->insertPlainText(datedata);

}


void MainWindow::on_actionExit_triggered()
{
    QMessageBox box;
          box.setWindowTitle(tr("Warning"));
          box.setIcon(QMessageBox::Warning);
          box.setText("Unsaved files will be lost!Continue?");

          QPushButton *yesBtn = box.addButton(tr("Yes"),
                                              QMessageBox::YesRole);
          box.addButton(tr("No"), QMessageBox::NoRole);
          QPushButton *cancelBut = box.addButton(tr("Cancel"),
                                             QMessageBox::RejectRole);

          box.exec();
          if (box.clickedButton() == yesBtn)
              QApplication::quit();
          if (box.clickedButton() == cancelBut)
               cout<<"No";
}


//main
#include "mainwindow.h"
#include "loginwindow.h"
#include "database.h"
#include <QApplication>
#include<QObject>
#include "database.h"

bool isLogin = false;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Database registerUser;
    w.showNormal();
    registerUser.create_database("users");
    return a.exec();
}

//encryption decryption
#include "database.h"
#include<iostream>
#include<fstream>
#include <filesystem>
#include<string.h>
#include <unistd.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

string encryptDecrypt(string toEncrypt) {
    char key = 'H'; //Any char will work
    string output = toEncrypt;

    for (int i = 0; i < toEncrypt.length(); i++)
    {
        output[i] = toEncrypt[i] ^ key;
    }

    return output;
}


//login
#include "login.h"
#include<iostream>
#include<string.h>
#include "database.h"
#include<fstream>
#include<filesystem>
#include <unistd.h>
#include <bits/stdc++.h>
#include <QMessageBox>
#include "encryptDecrypt.h"

using std::cout; using std::cin;
using std::endl; using std::string;
using std::filesystem::current_path;

using std::cout; using std::endl;
using std::system; using std::string;
namespace fs = std::filesystem;

string getUsername(string temp)
{
    string s = temp;
    string delimiter = "+";

    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        return token;
        s.erase(0, pos + delimiter.length());
    }
}
string getPassword(string temp)
{
    string s = temp;
    string delimiter = "+";
    int flag = 1;
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        if(flag == 2)
        {
            break;
        }
        flag++;
    }
    return token;
}

int login::loginUser(string username, string password)
{
    Database login;
    fstream file;
    string temp, un, pass;
    string path = current_path();
    path = path + "/Database/" + "users" + ".db";
    file.open(path,ios::in);
    while(getline(file, temp))
    {
        cout<<temp<<endl;
        temp = encryptDecrypt(temp);
        un = getUsername(temp);
        pass = getPassword(temp);
        cout<<un<<pass<<endl;
        if(un == username && pass == password)
        {
            return 1;
        }
    }
    return 0;
}

login::login()
{

}

login::~login()
{

}


//password
#include "spass.h"
#include<iostream>
#include<string.h>

using namespace std;

bool strongPassword(string pass)
{
    int flagD =0,flagS = 0,flagL=0;
    for(auto i=0;i<pass.length();i++)
    {
        if(pass.length()>=6)
        {
            flagL++;
        }
        if(isdigit(pass[i]))
        {
            flagD++;
        }
        if(!isdigit(pass[i]) && !isalpha(pass[i]))
        {
            flagS++;
        }
    }
    if(flagD>=1 && flagS>=1 && flagL >=1)
    {
        return true;
    }
    return false;
}


//database
#include "database.h"
#include<iostream>
//#include<fstream>
#include <filesystem>
#include<string.h>
#include <unistd.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encryptDecrypt.h"
#include<QFile>
#include<simplebuffer.h>
#include "login.h"

using std::cout; using std::cin;
using std::endl; using std::string;
using std::filesystem::current_path;


using std::cout; using std::endl;
using std::system; using std::string;
namespace fs = std::filesystem;

using namespace std;


Database::Database()
{
    string directory_name("Database");

        fs::create_directory(directory_name)?
        cout << "created directory - " << directory_name << endl :
        cout << "create_directory() failed" << endl;
}

Database::Database(string username)
{
    string path = current_path();
    string temp = path + "/Database/" + username;
    string directory_name(temp);
    fs::create_directory(directory_name)?
    cout << "created directory - " << directory_name << endl :
    cout << "create_directory() failed" << endl;
}

int Database::create_database(string dbname)
{
    fstream file;
    string path = current_path();
    path = path + "/Database/" + dbname + ".db";
    Database::databasename = path;
    file.open(path,ios::app);
    return 1;
}

/*int Database::create_database(string dbname, int flag)
{
    fstream file;
    string path = current_path();
    path = path + "/Database/" + dbname + ".db";
    Database::databasename = path;
    file.open(path,ios::app);
    return 1;
}*/

int Database::insert_data(string dbname,string data)
{
    fstream file;
    string path = current_path();
    path = path + "/Database/" + dbname + ".db";
    file.open(path,ios::app);
    file<<data<<"\n";
    file.close();
    return 1;
}

int Database::insert_data(string username,string dateandtime,QString data)
{
    fstream file;
    string path = current_path();
    path = path + "/Database/" + username + "/" + dateandtime + ".txt";
    //file.open(path, ios::out);
    //data.toStdString() = encryptDecrypt(data.toStdString());
    //file.close();
    QFile outfile;
    outfile.setFileName(QString::fromStdString(path));
    outfile.open(QIODevice::WriteOnly |QIODevice::Text);
    QTextStream out(&outfile);
    out<<data;
    outfile.close();
    return 1;
}

QString Database::show_data(string username,string dateandtime)
{
    fstream file;
    QString temp;
    string path = current_path();
    path = path + "/Database/" + username + "/" + dateandtime + ".txt";
    /*file.open(path, ios::in);
    file>>temp;
    temp = encryptDecrypt(temp);
    cout<<temp;
    file.close();*/
    QFile outfile;
    outfile.setFileName(QString::fromStdString(path));
    outfile.open(QIODevice::ReadOnly |QIODevice::Text);
    QTextStream in(&outfile);
    temp = in.readAll();
    outfile.close();
    QString decryptToString(temp);
    return temp;
}

