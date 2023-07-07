#ifndef VIEW_H
#define VIEW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QDialog
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    ~View();

private:
    void createDB();
    void createTable();
    void query();

private slots:
    void on_insertPushButton_clicked();

    void on_updatePushButton_clicked();

    void on_deletePushButton_clicked();

    void on_sortPushButton_clicked();

private:
    Ui::View *ui;
    // 与数据库建立的连接
    QSqlDatabase db;
    // 结果集
    QSqlQueryModel model;
};
#endif // VIEW_H
