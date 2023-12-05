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
#include <QFontDatabase>
#include <QPixmap>


Menu::Menu(Protocolo& protocolo) : protocolo(protocolo) {}

InformacionInicial Menu::ejecutar(int argc, char* argv[], bool& es_host) {
    InformacionInicial informacion;
    informacion.jugador = -1;
    salio = false;
    bool continuar_menu = true;
    int pos = 0;
    int sub_opcion;
    std::vector<id> opciones_partida;
    std::vector<RepresentacionMapa> opciones_mapa;
    QApplication app(argc, argv);

    QMainWindow *mainWindow = new QMainWindow();
    mainWindow->setWindowTitle("Worms");
    
    // creo letra "custom" en base a la tipografia que tenemos
    int fontId = QFontDatabase::addApplicationFont("assets/fonts/AdLibRegular.ttf");
    QString letraCustom = QFontDatabase::applicationFontFamilies(fontId).at(0);

    QStackedWidget *pantallas = new QStackedWidget();
    // seteo fondo
    pantallas->setStyleSheet("background-color: #cc8835;");
    // creo pagina principal
    QPixmap fotoLogo("assets/menu/WormsLogo.png");

    QWidget *paginaPrincipal = new QWidget();
    QVBoxLayout *verticalLayoutPrincipal = new QVBoxLayout(paginaPrincipal);
    QLabel *logo = new QLabel(paginaPrincipal);
    logo->setPixmap(fotoLogo);
    logo->setAlignment(Qt::AlignCenter);
    logo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); 
    verticalLayoutPrincipal->addWidget(logo);
    // agrego horizontal layout para centrar los botones
    QHBoxLayout *layoutHorizontalPrincipal = new QHBoxLayout();
    QVBoxLayout *botonesPrincipal = new QVBoxLayout();
    botonesPrincipal->addStretch();

    QPushButton *crearPartida = new QPushButton(paginaPrincipal);
    crearPartida->setText("Crear una partida");
    crearPartida->setFont(letraCustom);
    crearPartida->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    botonesPrincipal->addWidget(crearPartida);

    QPushButton *unirsePartida = new QPushButton(paginaPrincipal);
    unirsePartida->setText("Unirse a una partida");
    unirsePartida->setFont(letraCustom);
    unirsePartida->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); 
    botonesPrincipal->addWidget(unirsePartida);

    QPushButton *salir = new QPushButton(paginaPrincipal);
    salir->setText("Salir");
    salir->setFont(letraCustom);
    salir->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); 
    botonesPrincipal->addWidget(salir);

    botonesPrincipal->addStretch();

    QSpacerItem *espacioIzquierdaBotones = new QSpacerItem(100, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *espacioDerechaBotones = new QSpacerItem(100, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    layoutHorizontalPrincipal->addItem(espacioIzquierdaBotones);
    layoutHorizontalPrincipal->addLayout(botonesPrincipal);
    layoutHorizontalPrincipal->addItem(espacioDerechaBotones);
    verticalLayoutPrincipal->addLayout(layoutHorizontalPrincipal);


    // creo pagina crear
    QWidget *paginaCrear = new QWidget();
    QVBoxLayout *verticalLayoutCrear = new QVBoxLayout(paginaCrear);

    QLabel *logoCrear = new QLabel(paginaCrear);
    logoCrear->setPixmap(fotoLogo);
    logoCrear->setAlignment(Qt::AlignCenter);
    logoCrear->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); 
    verticalLayoutCrear->addWidget(logoCrear);

    QLabel *tituloCrear = new QLabel(paginaCrear);
    QFont fontTitulo;
    fontTitulo.setFamily(letraCustom);
    fontTitulo.setPointSize(20);
    fontTitulo.setWeight(50);
    tituloCrear->setFont(fontTitulo);
    tituloCrear->setText("Mapas disponibles:");
    tituloCrear->setAlignment(Qt::AlignCenter);
    verticalLayoutCrear->addWidget(tituloCrear);

    QHBoxLayout *layoutHorizontalCrear = new QHBoxLayout();
    QVBoxLayout *botonesCrear = new QVBoxLayout();

    QSpacerItem *espacioIzqBotonesCrear = new QSpacerItem(100, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *espacioDerBotonesCrear = new QSpacerItem(100, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    layoutHorizontalCrear->addItem(espacioIzqBotonesCrear);
    layoutHorizontalCrear->addLayout(botonesCrear);
    layoutHorizontalCrear->addItem(espacioDerBotonesCrear);
    verticalLayoutCrear->addLayout(layoutHorizontalCrear);


    // creo pagina unirse
    QWidget *paginaUnirse = new QWidget();
    QVBoxLayout *verticalLayoutUnirse = new QVBoxLayout(paginaUnirse);
    QLabel *logoUnirse = new QLabel(paginaUnirse);
    logoUnirse->setPixmap(fotoLogo);
    logoUnirse->setAlignment(Qt::AlignCenter);
    logoUnirse->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); 
    verticalLayoutUnirse->addWidget(logoUnirse);

    QLabel *tituloUnirse = new QLabel(paginaUnirse);
    tituloUnirse->setFont(fontTitulo);
    tituloUnirse->setText("Partidas disponibles:");
    tituloUnirse->setAlignment(Qt::AlignCenter);
    verticalLayoutUnirse->addWidget(tituloUnirse);

    QHBoxLayout *layoutHorizontalUnirse = new QHBoxLayout();
    QVBoxLayout *botonesUnirse = new QVBoxLayout();

    QSpacerItem *espacioIzqBotonesUnirse = new QSpacerItem(100, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem *espacioDerBotonesUnirse = new QSpacerItem(100, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    layoutHorizontalUnirse->addItem(espacioIzqBotonesUnirse);
    layoutHorizontalUnirse->addLayout(botonesUnirse);
    layoutHorizontalUnirse->addItem(espacioDerBotonesUnirse);
    verticalLayoutUnirse->addLayout(layoutHorizontalUnirse);



    // agrego paginas
    pantallas->addWidget(paginaPrincipal);
    pantallas->addWidget(paginaCrear);
    pantallas->addWidget(paginaUnirse);

    mainWindow->setCentralWidget(pantallas);


    QObject::connect(crearPartida, &QPushButton::clicked, [&]() {
        pantallas->setCurrentIndex(1);
        protocolo.pedirInformacion(MAPA);
        opciones_mapa = protocolo.obtenerMapas();

        botonesCrear->addStretch();
        for (int i = 0; i < (int)opciones_mapa.size(); i++) {
            QPushButton *mapa = new QPushButton(QString("%1. %2").arg(i).arg(QString::fromStdString(opciones_mapa[i].nombre)));
            mapa->setFont(letraCustom);
            mapa->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            botonesCrear->addWidget(mapa);
            QObject::connect(mapa, &QPushButton::clicked, [this, i, &informacion, &mainWindow, &opciones_mapa]() {
                informacion = this->protocolo.crearPartida(opciones_mapa[i].ID);
                mainWindow->close();
            });
        }
        botonesCrear->addStretch();
        es_host = true;
        QCoreApplication::processEvents();
    });

    QObject::connect(unirsePartida, &QPushButton::clicked, [&]() {
        pantallas->setCurrentIndex(2);
        protocolo.pedirInformacion(PARTIDA);
        opciones_partida = protocolo.obtenerPartidas();
        
        if (opciones_partida.size() == 0) {
            QFont fontTexto;
            fontTexto.setFamily(letraCustom);
            fontTexto.setPointSize(12);
            fontTexto.setWeight(40);
            QLabel *textoSinPartidas = new QLabel(paginaUnirse);
            textoSinPartidas->setFont(fontTexto);
            textoSinPartidas->setText("No hay partidas disponibles...");
            textoSinPartidas->setAlignment(Qt::AlignCenter);
            verticalLayoutUnirse->addWidget(textoSinPartidas);
            QSpacerItem *espacioUnirse = new QSpacerItem(100, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
            verticalLayoutUnirse->addItem(espacioUnirse);
        }
        botonesUnirse->addStretch();
        for (int i = 0; i < (int)opciones_partida.size(); i++) {
            QPushButton *partida = new QPushButton(QString("%1").arg(opciones_partida[i]));
            partida->setFont(letraCustom);
            partida->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            botonesUnirse->addWidget(partida);
            QObject::connect(partida, &QPushButton::clicked, [this, i, &informacion, &mainWindow, &opciones_partida]() {
                informacion = this->protocolo.unirseAPartida(opciones_partida[i]);
                mainWindow->close();
            });
        }
        botonesUnirse->addStretch();
        QCoreApplication::processEvents();
    });

    QObject::connect(salir, &QPushButton::clicked, [&]() {
        salio = true;
        mainWindow->close();
    });

    // Si se cierra la ventana, se sale del menu.
    QObject::connect(mainWindow, &QMainWindow::destroyed, [&]() {
        salio = true;
    });

    
    mainWindow->show();

    app.exec();
    return informacion;
}

bool Menu::salioDelMenu() {
    return salio;
}
