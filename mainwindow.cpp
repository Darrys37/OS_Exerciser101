#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "os.h"         // lấy thông tin hệ điều hành
#include "cpu_ram.h"    // lấy thông tin CPU và RAM
#include "storage.h"    // lấy thông tin Ổ cứng
#include "power.h"    // lấy thông tin pin
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QToolBar>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUi();
    fillOs();
    fillCpuRam();
    fillStorage();
    fillPower();

}
MainWindow::~MainWindow()
{
    delete ui;
}
static void prepareTwoColumnTable(QTableWidget *t)
{
    t->setColumnCount(2);
    QStringList headers{"Key", "Value"};
    t->setHorizontalHeaderLabels(headers);t->horizontalHeader()->setStretchLastSection(true);
    t->verticalHeader()->setVisible(false);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
}
void MainWindow::setupUi()
{
    setWindowTitle("Lab 1.01 – System Information (Qt + WinAPI)");
    auto *tb = addToolBar("Main");
    auto *actRefresh = tb->addAction("Refresh");
    connect(actRefresh, &QAction::triggered, this, &MainWindow::onRefreshClicked);

    auto *actExport = tb->addAction("Export JSON");
    connect(actExport, &QAction::triggered, this, &MainWindow::onExportJsonClicked);

    tabs_ = new QTabWidget(this);
    setCentralWidget(tabs_);
    tblOs_ = new QTableWidget(this);
    prepareTwoColumnTable(tblOs_);
    tblCpuRam_ = new QTableWidget(this);
    prepareTwoColumnTable(tblCpuRam_);
    tblStorage_ = new QTableWidget(this);
    tblStorage_->setColumnCount(5);
    tblStorage_->setHorizontalHeaderLabels({"Drive", "Type", "Total (GB)", "Free (GB)", "Free %"});
    tblStorage_->horizontalHeader()->setStretchLastSection(true);
    tblStorage_->verticalHeader()->setVisible(false);
    tblStorage_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblPower_ = new QTableWidget(this);
    prepareTwoColumnTable(tblPower_);
    tabs_->addTab(tblOs_, "OS");tabs_->addTab(tblCpuRam_, "CPU/RAM");
    tabs_->addTab(tblStorage_, "Storage");
    tabs_->addTab(tblPower_, "Power");
}

void MainWindow::fillOs()
{
    auto info = Os::getOsInfo();

    tblOs_->setRowCount(0);
    auto add = [&](const QString &k, const QString &v)
    {
        int r = tblOs_->rowCount();
        tblOs_->insertRow(r);
        tblOs_->setItem(r, 0, new QTableWidgetItem(k));
        tblOs_->setItem(r, 1, new QTableWidgetItem(v));
    };
    add("Product Name", info.productName);
    add("Version", info.versionString);
    add("Build", QString::number(info.buildNumber));
    add("Architecture", info.arch);
}
void MainWindow::fillCpuRam()
{
    auto si = cpu_ram::getSystemInfo();
    auto mem = cpu_ram::getMemoryStatus();

    tblCpuRam_->setRowCount(0);

    auto add = [&](const QString &k, const QString &v)
    {
        int r = tblCpuRam_->rowCount();
        tblCpuRam_->insertRow(r);
        tblCpuRam_->setItem(r, 0, new QTableWidgetItem(k));
        tblCpuRam_->setItem(r, 1, new QTableWidgetItem(v));
    };

    add("Processor Architecture", si.arch);
    add("Number of Processors", QString::number(si.numberOfProcessors));
    add("Total Physical Memory (GB)", QString::number(mem.totalPhysGB, 'f', 2));
    add("Available Physical Memory (GB)", QString::number(mem.availPhysGB, 'f', 2));
    add("Memory Load (%)", QString::number(mem.memoryLoad));
}
void MainWindow::fillStorage()
{
    auto drives = storage::getDrives();
    tblStorage_->setRowCount(0);
    for (const auto &d : drives){
        int r = tblStorage_->rowCount();
        tblStorage_->insertRow(r);
        tblStorage_->setItem(r, 0, new QTableWidgetItem(d.letter));
        tblStorage_->setItem(r, 1, new QTableWidgetItem(d.type));
        tblStorage_->setItem(r, 2, new QTableWidgetItem(QString::number(d.totalGB, 'f', 2)));
        tblStorage_->setItem(r, 3, new QTableWidgetItem(QString::number(d.freeGB, 'f', 2)));
        tblStorage_->setItem(r, 4, new QTableWidgetItem(QString::number(d.freePercent, 'f', 1)));
    }
    tblStorage_->resizeColumnsToContents();
}
void MainWindow::fillPower()
{
    auto p = power::getPowerStatus();tblPower_->setRowCount(0);
    auto add = [&](const QString &k, const QString &v)
    {
        int r = tblPower_->rowCount();
        tblPower_->insertRow(r);
        tblPower_->setItem(r, 0, new QTableWidgetItem(k));
        tblPower_->setItem(r, 1, new QTableWidgetItem(v));
    };
    add("AC Line Status", p.acLineStatus);
    add("Battery Flag", p.batteryFlag);
    add("Battery Life (%)", p.batteryPercent >= 0 ? QString::number(p.batteryPercent) : "Unknown");
}
void MainWindow::onRefreshClicked()
{
    qDebug() << "Refresh clicked!";
    fillOs();
    fillCpuRam();
    fillStorage();
    fillPower();
}
QJsonArray tableToJson(QTableWidget *table)
{
    QJsonArray array;
    int rows = table->rowCount();
    int cols = table->columnCount();

    for (int r = 0; r < rows; ++r) {
        QJsonObject rowObj;
        for (int c = 0; c < cols; ++c) {
            QString header = table->horizontalHeaderItem(c)->text();
            QString value = table->item(r, c) ? table->item(r, c)->text() : "";
            rowObj[header] = value;
        }
        array.append(rowObj);
    }
    return array;
}
void MainWindow::onExportJsonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save JSON", "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QJsonObject root;
    root["OS"] = tableToJson(tblOs_);
    root["CPU_RAM"] = tableToJson(tblCpuRam_);
    root["Storage"] = tableToJson(tblStorage_);
    root["Power"] = tableToJson(tblPower_);

    QJsonDocument doc(root);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file for writing");
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    QMessageBox::information(this, "Success", "Data exported to " + fileName);
}
