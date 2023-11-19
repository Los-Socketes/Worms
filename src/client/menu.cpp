#include "menu.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QStackedWidget>


Menu::Menu(Protocolo& protocolo) : protocolo(protocolo) {}

InformacionInicial Menu::ejecutar(int argc, char* argv[]) {
    InformacionInicial informacion;
    informacion.jugador = -1;
    bool continuar_menu = true;
    int pos = 0;
    int sub_opcion;
    std::vector<id> opciones_partida;
    std::vector<RepresentacionMapa> opciones_mapa;
    QApplication app(argc, argv);

    QMainWindow *mainWindow = new QMainWindow();
    mainWindow->setWindowTitle("Worms");
    mainWindow->resize(400, 300);

    QStackedWidget *pantallas = new QStackedWidget();
    // creo pagina principal
    QWidget *paginaPrincipal = new QWidget();
    QVBoxLayout *verticalLayoutPrincipal = new QVBoxLayout(paginaPrincipal);
    QLabel *titulo = new QLabel(paginaPrincipal);
    QFont font;
    font.setFamily(QString::fromUtf8("Noto Serif Thai"));
    font.setPointSize(80);
    font.setBold(true);
    font.setWeight(75);
    titulo->setFont(font);
    titulo->setText("WORMS");
    titulo->setAlignment(Qt::AlignCenter);
    verticalLayoutPrincipal->addWidget(titulo);

    QPushButton *crearPartida = new QPushButton(paginaPrincipal);
    crearPartida->setText("Crear una partida");
    verticalLayoutPrincipal->addWidget(crearPartida);

    QPushButton *unirsePartida = new QPushButton(paginaPrincipal);
    unirsePartida->setText("Unirse a una partida");
    verticalLayoutPrincipal->addWidget(unirsePartida);

    QPushButton *salir = new QPushButton(paginaPrincipal);
    salir->setText("Salir");
    verticalLayoutPrincipal->addWidget(salir);



    // creo pagina crear (prueba)
    QWidget *paginaCrear = new QWidget();
    QVBoxLayout *verticalLayoutCrear = new QVBoxLayout(paginaCrear);
    QPushButton *salirCrear = new QPushButton(paginaCrear);
    salirCrear->setText("Salir");
    verticalLayoutCrear->addWidget(salirCrear);

    // agrego primera pagina
    pantallas->addWidget(paginaPrincipal);
    // agrego pagina crear
    pantallas->addWidget(paginaCrear);

    mainWindow->setCentralWidget(pantallas);


    QObject::connect(crearPartida, &QPushButton::clicked, [&]() {
        pantallas->setCurrentIndex(1);
        QCoreApplication::processEvents();
        protocolo.pedirInformacion(MAPA);
        opciones_mapa = protocolo.obtenerMapas();
        std::cout << "Mapas disponibles:" << std::endl;
        for (const RepresentacionMapa& mapa : opciones_mapa) {
            std::cout << std::to_string(pos) << ") " << mapa.nombre << std::endl;
            pos++;
        }
        std::cin >> sub_opcion;
        informacion = protocolo.crearPartida(opciones_mapa[sub_opcion].ID);
        mainWindow->close();
    });

    QObject::connect(unirsePartida, &QPushButton::clicked, [&]() {
        protocolo.pedirInformacion(PARTIDA);
        opciones_partida = protocolo.obtenerPartidas();
        std::cout << "Partidas disponibles:" << std::endl;
        for (const id& id_partida : opciones_partida) {
            std::cout << std::to_string(pos) << ") " << id_partida << std::endl;
            pos++;
        }
        std::cin >> sub_opcion;
        informacion = protocolo.unirseAPartida(opciones_partida[sub_opcion]);
        mainWindow->close();
    });

    QObject::connect(salir, &QPushButton::clicked, [&]() {
        mainWindow->close(); // For simplicity, just close the application
    });

    
    mainWindow->show();

    app.exec();
    return informacion;
}

