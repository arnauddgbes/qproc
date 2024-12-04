#include <QCoreApplication>
#include <QProcess>
#include <QTextStream>

#include <filesystem>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    std::filesystem::path appfp = std::filesystem::canonical( "/proc/self/exe" ).parent_path();
    appfp.append( "runme" );
    const std::string appfnm = appfp.string();

    QProcess process;
    process.setProgram( QString(appfnm.c_str()) );
    QTextStream qout(stdout);

    bool hasoutput = false;
    int retcode = -1;

    // Connect signals to capture output and error streams
    QObject::connect(&process, &QProcess::readyReadStandardOutput, [&]() {
        const QByteArray result = process.readAllStandardOutput();
        if ( result.size() > 0 )
        {
            qout << result;
            qout.flush();
            hasoutput = true;
        }
    });

    QObject::connect(&process, &QProcess::readyReadStandardError, [&]() {
        const QByteArray errorOutput = process.readAllStandardError();
        qout << "Error: " << errorOutput;
        qout.flush();
    });

    QObject::connect(&process, &QProcess::finished,
                     [&]( int exitcode, QProcess::ExitStatus exitstatus ) {
        if ( exitstatus != QProcess::NormalExit || exitcode != 0 )
        {
            qout << "Finished with error: " << process.errorString();
            qout.flush();
            retcode = exitcode != 0 ? exitcode : 1;
        }
        else if ( !hasoutput )
        {
            const QByteArray result = process.readAllStandardOutput();
            if ( result.size() > 0 )
            {
                qout << result << "(in finished)";
                qout.flush();
                retcode = 0;
            }
            else
            {
                qout << "Empty output\n";
                qout.flush();
                retcode = 1;
            }
        }
        else
            retcode = 0;
    });

    // Start the process
    process.start();
    if ( !process.waitForStarted() )
    {
        qout << "Failed to start the process." << Qt::endl;
        QCoreApplication::exit( -1 );
    }

    // Wait for the process to finish
    if ( !process.waitForFinished() )
    {
        qout << "The process did not finish correctly." << Qt::endl;
        QCoreApplication::exit( -1 );
    }

    QCoreApplication::exit( retcode );
}
