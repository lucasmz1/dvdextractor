#include <QMainWindow>
#include <QProcess>
#include <QTimer>
#include <QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_vob_in_clicked();
    void on_pushButton_vob_out_clicked();
    void on_pushButton_run_clicked();

private:
    Ui::MainWindow *ui;
    QStringList vobsin;
    QString vobout;
    QProcess *process;
    QTimer *progressTimer;
};
