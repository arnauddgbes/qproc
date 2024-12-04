#include <QCoreApplication>
#include <QProcess>
#include <QTimer>

#include <filesystem>

class TestClass : public QObject
{
    Q_OBJECT
public:
    explicit TestClass(QObject* parent=nullptr);
             ~TestClass();

signals:

public slots:
    void    started();
    void    readyReadStandardOutput();
    void    readyReadStandardError();
    void    finished(int exitcode,QProcess::ExitStatus);
    void    error(QProcess::ProcessError);

private:
    void    createProcess();

    QProcess* process_ = nullptr;
    bool    hasoutput_ = false;
};


#include "main_async.moc"


TestClass::TestClass( QObject* parent )
    : QObject(parent)
{
    QTimer::singleShot( 200, this, &TestClass::createProcess );
}


TestClass::~TestClass()
{
    delete process_;
}


void TestClass::createProcess()
{
    std::filesystem::path appfp = std::filesystem::canonical( "/proc/self/exe" ).parent_path();
    appfp.append( "runme" );
    const std::string appfnm = appfp.string();

    delete process_;
    process_ = new QProcess();
    process_->setProgram( QString(appfnm.c_str()) );
    //connect( process_, SIGNAL(started()), this, SLOT(started()) );
    connect( process_, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()) );
    connect( process_, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()) );
    connect( process_, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)) );

    process_->start();
}


void TestClass::started()
{
    qDebug() << "Proc started";
}


void TestClass::readyReadStandardOutput()
{
    QProcess* process = (QProcess*)sender();
    const QByteArray result = process->readAllStandardOutput();
    if ( result.size() > 0 )
    {
        qInfo().noquote() << result;
        hasoutput_ = true;
    }
}


void TestClass::readyReadStandardError()
{
    QProcess* process = (QProcess*)sender();
    const QByteArray error = process->readAllStandardError();
    qWarning().noquote() << error;
}


void TestClass::finished( int exitcode, QProcess::ExitStatus exitstatus )
{
    QProcess* process = (QProcess*)sender();
    int retcode = -1;
    if ( exitstatus != QProcess::NormalExit || exitcode != 0 )
    {
        qDebug() << "Finished with error: " << process->errorString();
        retcode = exitcode != 0 ? exitcode : 1;
    }
    else if ( !hasoutput_ )
    {
        const QByteArray result = process->readAllStandardOutput();
        if ( result.size() > 0 )
        {
            qInfo().noquote() << result << "(in finished)";
            retcode = 0;
        }
        else
        {
            qDebug() << "Empty output";
            retcode = 1;
        }
    }
    else
        retcode = 0;

    QCoreApplication::exit( retcode );
}


void TestClass::error( QProcess::ProcessError error )
{
    qDebug() << "Error: " << error;
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    TestClass tester;
    return app.exec();
}
