#include "view.h"
#include "ui_view.h"
#include <QMessageBox>

View::View(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
    createDB();
    createTable();
    query();
}

View::~View()
{
    delete ui;
    db.close();
}

void View::createDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bookmark.db");
    if (!db.open()) {
        qDebug("打开数据库失败");
    } else {
        qDebug("打开数据库成功");
    }
}

void View::createTable()
{
    QSqlQuery query;
    QString str = "CREATE TABLE bookmark "
                  "(id INTEGER PRIMARY KEY, "
                  "class TEXT NOT NULL, "
                  "name TEXT NOT NULL, "
                  "website TEXT NOT NULL)";
    query.exec(str);
}

void View::query()
{
    QString str = "SELECT * FROM bookmark";
    model.setQuery(str);
    ui->tableView->setModel(&model);
    QHeaderView* header = ui->tableView->horizontalHeader();
    QAbstractItemModel* model = header->model();
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "分类");
    model->setHeaderData(2, Qt::Horizontal, "名称");
    model->setHeaderData(3, Qt::Horizontal, "网址");
    header->setModel(model);
    ui->tableView->setHorizontalHeader(header);
}

//插入书签
void View::on_insertPushButton_clicked()
{
    QSqlQuery query;
    query.prepare("INSERT INTO bookmark (class, name, website) "
                  "VALUES (?, ?, ?)");
    query.bindValue(0, ui->classLineEdit->text());
    query.bindValue(1, ui->nameLineEdit->text());
    query.bindValue(2, ui->websiteLineEdit->text());
    if (!query.exec()) {
        qDebug() << "插入失败, ERROR: " << query.lastError();
    }
    this->query();
}

//修改书签
void View::on_updatePushButton_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (index.row() < 0) {
        QMessageBox box(QMessageBox::Critical, "错误", "请选中书签行", QMessageBox::Ok, this);
        box.exec();
        return;
    }
    int id = index.siblingAtColumn(0).data().toInt();
    QSqlQuery query;
    query.prepare("UPDATE bookmark SET class=?,name=?,website=? WHERE id = ?");
    query.bindValue(0, ui->classLineEdit->text());
    query.bindValue(1, ui->nameLineEdit->text());
    query.bindValue(2, ui->websiteLineEdit->text());
    query.bindValue(3, id);
    if (!query.exec()) {
        qDebug() << "修改失败, ERROR: " << query.lastError();
    }
    this->query();
}

//删除书签
void View::on_deletePushButton_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (index.row() < 0) {
        QMessageBox box(QMessageBox::Critical, "错误", "请选中书签行", QMessageBox::Ok, this);
        box.exec();
        return;
    }

    int id = index.siblingAtColumn(0).data().toInt();
    QSqlQuery query;
    query.prepare("DELETE FROM bookmark WHERE id = ?");
    query.bindValue(0, id);
    if (!query.exec()) {
        //删除失败
        QMessageBox box(QMessageBox::Critical, "错误", "删除书签失败", QMessageBox::Ok, this);
        box.exec();
        return;
    }
    this->query();
}

//排序书签
void View::on_sortPushButton_clicked()
{
    QString condition;
    if (ui->conditionComboBox->currentText() == "升序") {
        condition = "ASC";
    } else if (ui->conditionComboBox->currentText() == "降序") {
        condition = "DESC";
    } else {
        //排序类型错误
        QMessageBox box(QMessageBox::Critical, "错误", "排序类型错误", QMessageBox::Ok, this);
        box.exec();
        return;
    }

    QString stmt = QString("SELECT * FROM bookmark ORDER BY id %2").arg(condition);
    model.setQuery(stmt);
    ui->tableView->setModel(&model);
}

