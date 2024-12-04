#include <QCoreApplication>
#include <QProcess>
#include <QTimer>

#include <filesystem>


static bool doMain()
{
    std::filesystem::path appfp = std::filesystem::canonical( "/proc/self/exe" ).parent_path();
    appfp.append( "runme" );
    const std::string appfnm = appfp.string();

    QProcess process;
    process.setProgram( QString(appfnm.c_str()) );
    process.start();
    if ( !process.waitForStarted() )
    {
        qDebug() << "Not started";
        return false;
    }

    if ( !process.waitForFinished() )
    {
        qDebug() << "Not finished";
        return false;
    }

    const QByteArray result = process.readAllStandardOutput();
    const QString output = QString::fromUtf8( result );
    const int exitcode = process.exitCode();
    if ( process.exitStatus() != QProcess::NormalExit || exitcode != 0 )
    {
        qDebug() << "Child process exited wit code " << exitcode;
        return false;
    }

    if ( output.size() < 1 )
    {
        qDebug() << "Empty output";
        return false;
    }

    qInfo().noquote() << output;

    return true;
}


static void doWork()
{
    const bool res = doMain();
    QCoreApplication::exit( res ? 0 : 1 );
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTimer::singleShot( 200, nullptr, doWork );
    return app.exec();
}
