#include "mainwindow.h"
Q_DECLARE_METATYPE(std::string)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    db = QSqlDatabase::addDatabase("QSQLITE");

    if(db.open()) // Si on a réussi à ouvrir la bdd
    {
        std::cout << "Vous êtes maintenant connecté à la BDD " << std::endl;

        executer_BDD(); // On execute la BDD (on la crée à partir d'un script SQL)

        connexion(); // On affiche la fenêtre de connexion
    }
    else
    {
        std::cout << "La connexion a échouée, désolé :(" << std::endl << q2c(db.lastError().text()) << std::endl;
    }

}

/* -----------------------------------------------------------------------------------
 *
 *                                  CONNEXION
 *
 * --------------------------------------------------------------------------------- */


void MainWindow::connexion() { // Afficher la fenêtre de connexion

    mainLayout = new QGridLayout;

    // On crée un QLineEdit pour l'adresse email, on l'a met par défaut pour aller plus vite
    m_pMail = new QLineEdit(this);
    m_pMail->setText("bmckeever0@google.fr");
    mainLayout->addWidget(m_pMail, 1, 0, 1, 1);

    // On crée un QLineEdit pour le mot de passe, on le met en mode password pour cacher les caractères
    m_pMotDePasse = new QLineEdit(this);
    m_pMotDePasse->setText("");
    mainLayout->addWidget(m_pMotDePasse, 3, 0, 1, 1);
    m_pMotDePasse->setEchoMode(QLineEdit::Password);

    // On crée un boutton pour de connecter
    m_pLogInButton = new QPushButton(tr("Se connecter"), this);
    connect(m_pLogInButton, &QPushButton::clicked, this,&MainWindow::OnClickedLogIn);
    mainLayout->addWidget(m_pLogInButton, 4, 0);
    m_pLogInButton->setMaximumSize(150, 50);

    // On crée deux QLabel pour indiquer l'email et le mot de passe
    titreMail = new QLabel(tr("Saisissez votre email"), this);
    mainLayout->addWidget(titreMail, 0, 0);
    titreMotDePasse = new QLabel(tr("Saisissez votre mot de passe"), this);
    mainLayout->addWidget(titreMotDePasse, 2, 0);

    // On montre la langue du logiciel dans le statusBar
    this->statusBar()->showMessage(statu_bar);

    // On définit le titre de la fenêtre
    setWindowTitle(tr("Se connecter"));


    this->setCentralWidget (new QWidget (this)) ;
    this->centralWidget()->setLayout(mainLayout);

}

void MainWindow::OnClickedLogIn() { // Vérification de la connection

    // On effectue une requete avec les infos des QLineEdit
    QSqlQuery query = RequeteSQL("SELECT id FROM employes WHERE email = '" + m_pMail->text() +
                                     "' AND motdepasse = '" + m_pMotDePasse->text() + "'");

    if (query.next()) { // Si la requete a retourné des valuers c'est qu'on a trouvé une correspondance
        id_employes = query.value(0).toString().toInt(); // On garde en mémoire l'id de l'employé connecté
        //connecter = true;
        DeleteLogInWindow(); // On supprime la fenetre de Connexion
        ShowMenuBar(); // On affiche le menu
        afficher_requete_libre(); // On affiche la premiere fenetre "Requete libre"
    }
}

void MainWindow::DeleteLogInWindow() { // Suppression des éléments de connexion

    delete m_pMail;
    delete m_pMotDePasse;
    delete m_pLogInButton;
    delete titreMail;
    delete titreMotDePasse;

}

void MainWindow::CleanMainWindow() { // Néttoyage de la fenêtre

    setWindowTitle(tr("Base de donnée Magasin")); // On définit le titre de la fenêtre

    // Pour tous les Widget de la fenêtre, on l'enleverra et le supprimera
    QLayoutItem *child;
    while( (child = mainLayout->takeAt( 0 )) )
    {
        mainLayout->removeItem( child );
        delete child->widget();
        delete child;
     }

}



/* -----------------------------------------------------------------------------------
 *
 *                                  DETAILS CONNEXION, MENU
 *
 * --------------------------------------------------------------------------------- */



void MainWindow::afficher_detail_connexion() { // Affiche les infos de l'employé connecté

    QString texte;

    // On fait une requête pour obtenir les infos de l'employé enregistré dans la BDD
    QSqlQuery query = RequeteSQL("SELECT e.nom, e.titre_poste, m.code_postal "
                                 "FROM employes e INNER JOIN magasins m ON e.id_magasin = m.id WHERE e.id = '"
                                 + QString::number(id_employes) + "'" );


    // On récupère les retours de la BDD et on met sa dans un QString
    query.next();

    texte = query.value(0).toString() + " : " + query.value(1).toString() + " (" + query.value(2).toString() + ")";

    // On crée un QLabel qui contiendra les infos de l'employé (Nom, fonction et code postal du magasin)
    DetailConnexion = new QLabel(" ", this);
    DetailConnexion->setText(texte);
    mainLayout->addWidget(DetailConnexion, 0, 0);

}

void MainWindow::ShowMenuBar() { // Initialisation et affichage du menu

    // On crée une QAction Requête Libre avec une icône, on la connecte au slot afficher_requete_libre()
    requete_libre = new QAction(tr("&Requête libre"), this);
    requete_libre->setIcon(QIcon("/Users/Victor/Projet_QT_BDD/requete.png"));
    connect(requete_libre, SIGNAL(triggered()), this, SLOT(afficher_requete_libre()));

    // On crée une QAction Recherche Table avec une icône, on la connecte au slot afficher_requete_BDD()
    recherche_table = new QAction(tr("&Rechercher dans la base de donnée"), this);
    recherche_table->setIcon(QIcon("/Users/Victor/Projet_QT_BDD/rechercher.png"));
    connect(recherche_table, SIGNAL(triggered()), this, SLOT(afficher_rechercher_BDD()));

    // On crée une QAction Enregistrer Commande avec une icône, on la connecte au slot afficher_enregistrer_commande()
    enregistrer_commande = new QAction(tr("&Enregistrer une commande"), this);
    enregistrer_commande->setIcon(QIcon("/Users/Victor/Projet_QT_BDD/panier.png"));
    connect(enregistrer_commande, SIGNAL(triggered()), this, SLOT(afficher_enregistrer_commande()));

    // On crée une QAction Recherche Magasin avec une icône, on la connecte au slot afficher_rechercher_magasin()
    recherche_magasin = new QAction(tr("&Rechercher dans le magasin"), this);
    recherche_magasin->setIcon(QIcon("/Users/Victor/Projet_QT_BDD/magasin.png"));
    connect(recherche_magasin, SIGNAL(triggered()), this, SLOT(afficher_rechercher_magasin()));

    // Création de la toolBar et ajout des Action
    QToolBar *toolBarMenu = this->addToolBar("Menu");
    toolBarMenu->addAction(requete_libre);
    toolBarMenu->addAction(recherche_table);
    toolBarMenu->addAction(enregistrer_commande);
    toolBarMenu->addAction(recherche_magasin);


    fichier = menuBar()->addMenu(tr("&Fichier")); // On crée une menu bar

    // On crée une QAction Charger Une Requête avec un raccourci, une icone et on la connecte au slot chargerRequete()
    QAction *actionRequete = new QAction(tr("Charger une requête"), this);
    actionRequete->setShortcut(QKeySequence("Ctrl+R"));
    actionRequete->setIcon(QIcon("/Users/Victor/Projet_QT_BDD/open.png"));
    this->connect(actionRequete, SIGNAL(triggered()), this, SLOT(chargerRequete()));
    fichier->addAction(actionRequete);

    // On crée une QAction Sauvergarder Une Requête avec un raccourci, une icone et la connecte au slot
    QAction *actionSauvegarde = new QAction(tr("Sauvegarder le résultat"), this);
    actionSauvegarde->setShortcut(QKeySequence("Ctrl+S"));
    actionSauvegarde->setIcon(QIcon("/Users/Victor/Projet_QT_BDD/save.png"));
    this->connect(actionSauvegarde, SIGNAL(triggered()), this, SLOT(sauvegarderRequete()));
    fichier->addAction(actionSauvegarde);

    // Création d'un menu En Savoir Plus
    aide = menuBar()->addMenu(tr("&Aide"));

    QAction *actionAide1 = new QAction(tr("Contenu"), this);
    this->connect(actionAide1, SIGNAL(triggered()), this, SLOT(afficherContenu()));
    aide->addAction(actionAide1);

    QAction *actionAide2 = new QAction(tr("À propos de"), this);
    this->connect(actionAide2, SIGNAL(triggered()), this, SLOT(afficherAProposDe()));
    aide->addAction(actionAide2);

    // Création d'un menu En Savoir Plus
    langue = menuBar()->addMenu(tr("&Langue"));

    QAction *actionLangue1 = new QAction("Français", this);
    this->connect(actionLangue1, SIGNAL(triggered()), this, SLOT(changeLangueFR()));
    actionLangue1->setIcon(QIcon("/Users/Victor/Projet_QT_BDD/france.png"));
    langue->addAction(actionLangue1);

    QAction *actionLangue2 = new QAction("English", this);
    this->connect(actionLangue2, SIGNAL(triggered()), this, SLOT(changeLangueEN()));
    actionLangue2->setIcon(QIcon("/Users/Victor/Projet_QT_BDD/english.png"));
    langue->addAction(actionLangue2);

}

void MainWindow::chargerRequete() { // Charge une requête dans un fichier .txt

    // On ouvre une fenêtre pour chercher le fichier texte contenant la requête
    QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir une requête"), "User/", tr("Fichier texte (*.txt)"));
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) { // Si l'ouverture du fichier est réussi
        QTextStream flux(&file); // On crée un flux
        QString tout = flux.readAll(); // On copie toute la requête dans un QString
        afficher_requete_libre(); // On affiche la fenêtre correspondant à une requête libre
        m_pRequeteLibre->setText(tout); // On ajoute au TextEdit de cette fenêtre
    }

    file.close(); // On ferme le fichier ouvert

}

void MainWindow::sauvegarderRequete() { // Sauvegarde une requête dans un fichier .txt

    // On ouvre une fenêtre pour sauvegarder le résultat dans un fichier texte

    QString fileName = QFileDialog::getSaveFileName(this, tr("Sauvegarde"), "", tr("Fichier texte (*.txt)"));
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) { // Si l'ouverture du fichier est réussi


        QString writeInFile; // On crée un QString qui sera l'écriture du tableau
        QTextStream flux(&file); // On crée un flux

        // On fait une boucle en parcourant le header horizontal du tableau
        for (int i=0; i < tableau_reponse->columnCount(); i++) {
            // On récupère chaque item du header
            QTableWidgetItem *item = tableau_reponse->horizontalHeaderItem(i);

            // S'il n'est pas vide on le rajoute au QString
            if (item!=NULL) {
                writeInFile += item->text() + "/";
            }
        }

        // On enlève le '/' de trop et on rajoute un ';\n' pour signaler la fin de la ligne
        writeInFile.truncate(writeInFile.size()-1);
        writeInFile += ";\n";

        // On fait une boucle pour obtenir chaque ligne du tableau
        for (int i=0; i < tableau_reponse->rowCount(); i++) {

            // On fait une boucle pour obtenir chaque colonne d'une ligne du tableau
            for (int y=0; y < tableau_reponse->columnCount(); y++) {

                // On récupère chaque item de la ligne
                QTableWidgetItem *item = tableau_reponse->item(i,y);

                // S'il n'est pas nul on le rajoute au QString
                if (item!=NULL) {
                    writeInFile += item->text() + "/";
                }

            }

            // On enlève le '/' de trop et on rajoute un ';\n' pour signaler la fin de la ligne
            writeInFile.truncate(writeInFile.size()-1);
            writeInFile += ";\n";

        }

        // On écrit notre QString dans le fichier de sauvegarde
        flux << writeInFile << endl;

    }

    file.close(); // On ferme le fichier ouvert

}

void MainWindow::afficherContenu() {

    QString message = tr("Contenu :\n"
                      "\n"
                      "L'application est divisé en 4 fenêtres distincts accéssible depuis le menu tool bar.\n"
                      "•   L'icône '?' permet de faire une requête libre dans la base de donnée de tous les magasins.\n"
                      "•   L'icône 'loupe' permet de faire une recherche plus précise dans la base de "
                      "donnée de tous les magasins.\n"
                      "•   L'icône 'panier' permet d'effectuer une commande en fonction du produit et de la "
                      "quantité demandé. L'application gère également les stocks et ne permet pas les "
                      "stocks négatifs.\n"
                      "•   L'icône 'magasin' permet une recherche dans le stock d'un magasin précis.\n"
                      "\n"
                      "Le menu permet de charger une requête depuis un fichier texte externe, ou bien de sauvegarder "
                      "le résultat d'une requête éffectué depuis l'une des 4 fenêtres.\n"
                      "\n");

    QMessageBox::about(this, tr("Contenu"), message);

}

void MainWindow::afficherAProposDe() {

    QString message = tr("À Propos de :\n"
                      "\n"
                      "Application de gérance d'une base de donnée d'un magasin.\n"
                      "Développé en partenariat avec l'école d'ingénieurie ESME Sudria Ivry-Sur-Seine.\n"
                      "Développé par les étudiants :\n"
                      "- Deshayes Guillaume,\n"
                      "- Fauquembergue Victor,\n"
                      "- Koenig Julien.\n"
                      "Développé sous Qt 5.9.2 et présenté en démo sous macOS Sierra (10.12).\n"
                      "\n"
                      "Copyright, sous réserve de droit - Décembre 2017.");

    QMessageBox::about(this, tr("À Propos De"), message);

}

void MainWindow::changeLangueEN() {

    if (translator.load("/Users/Victor/Projet_QT_BDD/test_en_EN")) {
        qApp->installTranslator(&translator);
        statu_bar = "en_EN";
        this->statusBar()->showMessage(statu_bar);
    }

}

void MainWindow::changeLangueFR() {

    qApp->removeTranslator(&translator);
    statu_bar = "fr_FR";
    this->statusBar()->showMessage(statu_bar);

}



/* -----------------------------------------------------------------------------------
 *
 *                                  REQUÊTE LIBRE
 *
 * --------------------------------------------------------------------------------- */



void MainWindow::afficher_requete_libre() { // Affiche la fenêtre Requête Libre

    // Remise à zéro de la page
    CleanMainWindow();

    // Affiche le detail de la connexion
    afficher_detail_connexion();

    /* ---- Requete libre ---- */

    // Création d'un QLabel Éxecuter Une Requête
    titreRequeteLibre = new QLabel(tr("Éxecuter une requête"), this);
    mainLayout->addWidget(titreRequeteLibre, 1, 0);

    // Création d'un QTextEdit et initialisation à SELECT * FROM
    m_pRequeteLibre = new QTextEdit(this);
    m_pRequeteLibre->setText("SELECT * FROM ");
    mainLayout->addWidget(m_pRequeteLibre, 2, 0, 1, 5);
    m_pRequeteLibre->setMaximumSize(900, 100);

    // Création d'un boutton Envoyer La Requête
    m_pSendRequeteLibre = new QPushButton(tr("Envoyer le requête"), this);
    connect(m_pSendRequeteLibre, &QPushButton::clicked, this,&MainWindow::OnClickedRequeteLibre);
    mainLayout->addWidget(m_pSendRequeteLibre, 3, 0);
    m_pSendRequeteLibre->setMaximumSize(150, 50);

    // Création d'un boutton SELECT
    m_pSELECTRequeteLibre = new QPushButton("SELECT", this);
    connect(m_pSELECTRequeteLibre, &QPushButton::clicked, this,&MainWindow::OnClickedButtonRequeteLibre);
    mainLayout->addWidget(m_pSELECTRequeteLibre, 3, 1);

    // Création d'un boutton FROM
    m_pFROMRequeteLibre = new QPushButton("FROM", this);
    connect(m_pFROMRequeteLibre, &QPushButton::clicked, this,&MainWindow::OnClickedButtonRequeteLibre);
    mainLayout->addWidget(m_pFROMRequeteLibre, 3, 2);

    // Création d'un boutton WHERE
    m_pWHERERequeteLibre = new QPushButton("WHERE", this);
    connect(m_pWHERERequeteLibre, &QPushButton::clicked, this,&MainWindow::OnClickedButtonRequeteLibre);
    mainLayout->addWidget(m_pWHERERequeteLibre, 3, 3);

    // Création d'un boutton DELETE
    m_pDELETERequeteLibre = new QPushButton("DELETE", this);
    connect(m_pDELETERequeteLibre, &QPushButton::clicked, this,&MainWindow::OnClickedButtonRequeteLibre);
    mainLayout->addWidget(m_pDELETERequeteLibre, 3, 4);

    // On montre la langue du logiciel dans le statusBar
    this->statusBar()->showMessage(statu_bar);


    menuBar()->setNativeMenuBar(false); // Permet de mettre la menu bar dans la fenetre (comme sur windows)
    this->setCentralWidget (new QWidget (this)) ;
    this->centralWidget()->setLayout(mainLayout);

}

void MainWindow::OnClickedRequeteLibre() { // Effectue une requête et l'affiche dans la fenêtre

    // Notre QTableWidget pointera sur un QTableWidget remplie des retours de la requête
    tableau_reponse = &TableRequeteSQL(m_pRequeteLibre->toPlainText());

    // On affiche le QTableWidget
    mainLayout->addWidget(tableau_reponse, 5, 0, 1, 0);

}

void MainWindow::OnClickedButtonRequeteLibre() { // Ajoute le texte des bouttons dans la zone de texte

    // On crée un boutton qui pointera sur celui qui vient d'être cliqué
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());

    // On crée un QString
    QString texte;

    // Le texte prend la valeur du QTextEdit m_pRequeteLibre, on y ajoute le texte du boutton cliqué
    texte = m_pRequeteLibre->toPlainText();
    texte += clickedButton->text() + " ";

    // Le QTextEdit prend la valeur du QString
    m_pRequeteLibre->setText(texte);

}



/* -----------------------------------------------------------------------------------
 *
 *                                  RECHERCHE BDD
 *
 * --------------------------------------------------------------------------------- */



void MainWindow::afficher_rechercher_BDD() { // Afficher la fenêtre Rechercher BDD

    // Remise à zéro de la page
    CleanMainWindow();

    // Affiche le detail de la connexion
    afficher_detail_connexion();

     /*---- Rechercher BDD ---- */

    // Création d'un QComboBox avec toutes les tables dans la BDD
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem("approvisionnement");
    comboBox->addItem("clients");
    comboBox->addItem("commandes");
    comboBox->addItem("detail_commande");
    comboBox->addItem("employes");
    comboBox->addItem("magasins");
    comboBox->addItem("produits");
    comboBox->addItem("stocks");

    // On fait pointer notre QComboBox global sur celui-ci
    choixBDD = comboBox;

    // On le connecte au slot ChangedIndexRechercheBDD()
    QObject::connect(choixBDD, SIGNAL(currentIndexChanged(QString)), this, SLOT(ChangedIndexRechercheBDD()));
    mainLayout->addWidget(choixBDD, 2, 0, 2, 1);

    // Création d'un QLabel Rechercher Une Requête
    titreRequeteLibre = new QLabel(tr("Rechercher une requête"), this);
    mainLayout->addWidget(titreRequeteLibre, 1, 0);

    // Création d'un boutton Rechercher
    m_pSendRechercheBDD = new QPushButton(tr("Rechercher"), this);
    connect(m_pSendRechercheBDD, &QPushButton::clicked, this,&MainWindow::OnClickedButtonRechercheBDD);
    mainLayout->addWidget(m_pSendRechercheBDD, 3, 2);

    // On montre la langue du logiciel dans le statusBar
    this->statusBar()->showMessage(statu_bar);


    menuBar()->setNativeMenuBar(false); // Permet de mettre la menu bar dans la fenetre (comme sur windows)
    this->setCentralWidget (new QWidget (this)) ;
    this->centralWidget()->setLayout(mainLayout);

}

void MainWindow::ChangedIndexRechercheBDD() { // Appellé lors d'une sélection d'un QComboBox, remplie et
    // affiche un QListWidget

    // On crée un QListBox
    checkedBox = new QListWidget;

    // On remet à 0 strListRechercheBDD
    strListRechercheBDD.clear();

    // Selon le choix de la table, on remplie la liste de recherche par ses colonnes
    if (q2c(choixBDD->currentText()) == "approvisionnement") {

        strListRechercheBDD << "Tout" << "numMagasin" << "numProduit" << "numFournisseur" << "date_livraison"
                << "prix_achat_unitaire";
    }
    else if (q2c(choixBDD->currentText()) == "clients") {

        strListRechercheBDD << "Tout" << "id" << "nom" << "date_naissance" << "email"
                << "telephone" << "adresse" << "code_postal" << "pays" << "date_inscription" << "carte_fidelite";

    }
    else if (q2c(choixBDD->currentText()) == "commandes") {

        strListRechercheBDD << "Tout" << "id" << "idClient" << "idMagasin" << "date_commande"
                << "EmployeResponsableVente" << "mode_paiement";

    }
    else if (q2c(choixBDD->currentText()) == "detail_commande") {

        strListRechercheBDD << "Tout" << "idCommande" << "idProduit" << "quantite";

    }
    else if (q2c(choixBDD->currentText()) == "employes") {

        strListRechercheBDD << "Tout" << "id" << "nom" << "date_naissance" << "email"
                << "telephone" << "adresse" << "code_postal" << "pays"
                << "date_recrutement" << "titre_poste" << "id_magasin";

    }
    else if (q2c(choixBDD->currentText()) == "magasins") {

        strListRechercheBDD << "Tout" << "id" << "adresse" << "code_postal" << "telephone"
                << "pays" << "date_ouverture";

    }
    else if (q2c(choixBDD->currentText()) == "produits") {

        strListRechercheBDD << "Tout" << "id" << "nom" << "categorie" << "description"
                << "prix_vente_unitaire";

    }
    else if (q2c(choixBDD->currentText()) == "stocks") {

        strListRechercheBDD << "Tout" << "idProduit" << "idMagasin";

    }

    // On ajoute la liste au QListWidget, chaque élément de la liste sera un QListWidgetItem
    checkedBox->addItems(strListRechercheBDD);

    // On fait une boucle dans la liste
    QListWidgetItem* item = 0;
    for(int i = 0; i < checkedBox->count(); ++i){
        item = checkedBox->item(i); // Chaque item devien un élément sélectionnable
        item->setCheckState(Qt::Unchecked); // Chaque item est défini comme non-sélectionné
    }

    // On ajoute la checkedBox dans la fenêtre
    mainLayout->addWidget(checkedBox, 2, 1, 2, 1);

}

void MainWindow::OnClickedButtonRechercheBDD() { // On effectue une recherche des champs sélectionné et on
    // ajoute des filtres

    boolLineEdit1 = false;
    boolSlider1 = false;

    // On cache les filtres au cas où ils seraient déjà activé
    slider1RechercheBDD->hide();
    label1RechercheBDD->hide();
    lineEdit1RechercheBDD->hide();
    label2RechercheBDD->hide();

    // On crée une requête ainsi qu'une liste sous QString
    QString texte = "SELECT ";
    QString colonne = "";

    // Pour chaque item dans la liste, s'il est coché on l'ajoute à la requête et à la liste
    for (int i = 0; i < checkedBox->count(); ++i) {
        QListWidgetItem *item = checkedBox->item(i);
        if (item->checkState() == Qt::Checked) {
            texte += item->text() + ", ";
            colonne += item->text() + "/";
        }
    }

    // Si la case "Tout" est sélectionné on remplace le texte par '*' symbolisant le tout en SQL
    if (texte.contains("Tout"))
        texte = "SELECT *";
    else // Sinon on supprime les deux derniers caractère (un ' et un espace)
        texte.truncate(texte.size()-2);

    // On ajoute FROM et la table choisi
    texte += " FROM " + choixBDD->currentText();

    // On effectue une requête avec les paramètre demandés
    //QTableWidget *tableWidget = new QTableWidget;
    tableau_reponse = &TableRequeteSQL(texte);

    // On récupère une table (liste à 2 dimensions de tout les types des colonnes sélectionnées en paramètre
    listTypeSQL = ReturnTableSQL(colonne);

    // Pour chaque type de la table
    for (int i= 0; i < listTypeSQL.size(); i++) {
        // On récupère chaque ligne de la table sous forme de liste
        QStringList listName = listTypeSQL[i].split("/");

        // Si la taille de la liste est supérieure à 3
        if (listName.size() > 3) {
            // On regarde si c'est un int et si on n'a pas déjà initialisé un slider
            if (listName[3] == "int" and boolSlider1 == false) {

                // On initialise le slider avec min et max et le nom de la colonne
                SetSliderRechercheBDD(listName[1].toInt(),listName[2].toInt(), listName[0]);

                // On ajoute le slider à la fenêtre
                mainLayout->addWidget(slider1RechercheBDD, 6, 0, 1, 0);

                // On predéfini une taille maximal pour le slider
                slider1RechercheBDD->setMaximumSize(200, 50);

                boolSlider1 = true;
            }
        }
        else if (listName.size() > 1) { // Sinon si la taille est supérieure à 1
            // Si c'est un char et qu'on n'a pas déjà initialisé le QlineEdit
            if (listName[1] == "char" and boolLineEdit1 == false) {

                // On crée un QLineEdit
                lineEdit1RechercheBDD = new QLineEdit;
                lineEdit1RechercheBDD->setText("");

                // On crée un QLabel avec le nom de la colonne
                label2RechercheBDD = new QLabel;
                label2RechercheBDD->setText(listName[0]);

                // On ajoute le QLineEdit et le QLabel à la fenêtre
                mainLayout->addWidget(lineEdit1RechercheBDD, 6, 1, 1, 1);
                mainLayout->addWidget(label2RechercheBDD, 5, 1, 1, 1);

                boolLineEdit1 = true;
            }
        }
    }

    if (boolLineEdit1 == false) {
        lineEdit1RechercheBDD->hide();
        label2RechercheBDD->hide();
    }

    // On crée un boutton Filtrer connecté à la méthode OnClickedFiltreRechercheBDD et on l'ajoute à la fenêtre
    buttonFiltreRechercheBDD = new QPushButton("Filtrer", this);
    connect(buttonFiltreRechercheBDD, &QPushButton::clicked, this, &MainWindow::OnClickedFiltreRechercheBDD);
    mainLayout->addWidget(buttonFiltreRechercheBDD, 2, 2);

    // On crée un QLabel Filtres qu'on affiche dans la fenêtre
    labelTitreFiltreRechercheBDD->setText("Filtres : ");
    mainLayout->addWidget(labelTitreFiltreRechercheBDD, 4, 0);

    // On ajoute la réponse de la requête dans la fenêtre
    mainLayout->addWidget(tableau_reponse, 7, 0, 1, 0);

}

void MainWindow::OnClickedFiltreRechercheBDD() { // Méthode qui se lance lorsque le bouton "Filtre" est appuyé

    int countInt = 0, countChar = 0;

    QString requete = "SELECT "; // Correspond au début d'une requête (ex SELECT id)
    QString requete2 = " WHERE "; // Correspond à la fin d'une requête (ex WHERE id < 8)

    for (int i=0; i<listTypeSQL.size(); i++) { // On parcours la liste des colonnes séléctionnées
        //ainsi que leur types associées

        QStringList listName = listTypeSQL[i].split("/"); // On partitionne les infos

        requete += listName[0] + ", ";

        if (listName.size() > 3) { // S'il y a plus de 3 infos
            if (listName[3] == "int" and countInt == 0) { // Si c'est un type int et que c'est le premier
                // Si ce n'est pas le premier filtre alors on rajoute un 'AND' à la requête
                if (i > 0)
                    requete2 += " AND ";

                // On ajoute à la fin de la requête le filtre, la condition inférieur
                requete2 += listName[0];
                requete2 += " < " + QString::number(slider1RechercheBDD->value());

                countInt ++;
            }
        }
        else if (listName.size() > 1) { // Sinon si y a moins de 3 mais plus de 1 infos
            if (listName[1] == "char" and countChar == 0) { // Si c'est un type char et que c'est le premier
                if (lineEdit1RechercheBDD->text() != "") { // Si le champs de filtre a été saisi
                    if (i > 0)
                        requete2 += " AND ";

                    // On ajoute à la fin de la requête le filtre, si le champs entré est contenu dans la table
                    requete2 += listName[0];
                    requete2 += " LIKE '%" + lineEdit1RechercheBDD->text() + "%'";
                    countChar++;
                }
            }
        }

    }

    // On supprime les 4 derniers caractères de la première requête ( ", ")
    requete.truncate(requete.size()-4);

    // On effectue la requête
    //QTableWidget *tableWidget = new QTableWidget;
    tableau_reponse = &TableRequeteSQL(requete + " FROM " + choixBDD->currentText() + requete2);

    // On affiche le résultat de la requête dans la fenêtre
    mainLayout->addWidget(tableau_reponse, 7, 0, 1, 0);

}

QStringList MainWindow::ReturnTableSQL(QString texte) { // Retourne les type des colonnes d'une table

    // On partitionne le QString texte
    QStringList colonne = texte.split("/");
    QSqlQuery query;
    int i=0;

    executer_BDD(); // Chelou !!!

    // On effectue une requête PRAGMA qui renvoie les infos de la table dont les types des colonnes
    if(query.exec("PRAGMA table_info(" + choixBDD->currentText() + ")"))
    {
        // Tant qu'il y a une nouvelle ligne dans la réponse à la requête
        while(query.next() && i<colonne.size()-1)
        {
            // Si le nom de la colonne correspond à la requête
            if (colonne[i] == query.value(1).toString()) {

                // Si c'est un int
                if (query.value(2).toString().contains("int") == 1) {

                    // On effectue une nouvelle requête pour chercher le max et le min de la colonne
                    QSqlQuery query2;
                    if(query2.exec("SELECT MIN(" + colonne[i] + "), MAX(" + colonne[i] + ") "
                                   "FROM " + choixBDD->currentText())) {

                        // On rajoute le max et le min dans la liste
                        while (query2.next()) {
                           colonne[i] += "/" + query2.value(0).toString();
                           colonne[i] += "/" + query2.value(1).toString();
                        }
                    }
                    colonne[i] += "/int";
                }
                else if (query.value(2).toString().contains("char"))
                    // Si c'est un char (ou varchar ...) on ajoute char à la liste
                    colonne[i] += "/char";
            }
            i++;
        }
    }
    else
    {
        std::cout << "Une erreur s'est produite. :(" << std::endl << q2c(query.lastError().text()) << std::endl;
    }

    // On retourne la liste
    return colonne;

}

void MainWindow::SetSliderRechercheBDD(int min, int max, QString name) { // Initialise un QSlider

    // On initialise le QSlider
    slider1RechercheBDD = new QSlider( Qt::Horizontal, this );
    // On définit le maximum et le minimum
    slider1RechercheBDD->setMaximum(max);
    slider1RechercheBDD->setMinimum(min);
    //On définit la position initial
    slider1RechercheBDD->setSliderPosition(max);

    // On crée un QLabel qui affichera l'état du QSlider et on l'ajoute à la fenêtre
    label1RechercheBDD = new QLabel;
    label1RechercheBDD->setText(name + " : < " + QString::number(max));
    mainLayout->addWidget(label1RechercheBDD, 5, 0, 1, 0);

    // On connecte le QSlider au slot ChangedSliderRechercheBDD avec le signal émis
    // dès qu'il y a un changement de valeur du QSLider
    QObject::connect(slider1RechercheBDD, SIGNAL(valueChanged(int)),this, SLOT(ChangedSliderRechercheBDD(int)));

}

void MainWindow::ChangedSliderRechercheBDD (int value) { // Met à jour le QLabel du QSlider

    // On récupère la valeur du QSlider
    QString texte = label1RechercheBDD->text();
    // On récupère le nom du QLabel
    QStringList list = texte.split(":");
    // On met à jour le QLabel avec la nouvelle valeur de QSlider
    label1RechercheBDD->setText(list[0] + ": < " + QString::number(value));

}


/* -----------------------------------------------------------------------------------
 *
 *                                  ENREGISTRER COMMANDE
 *
 * --------------------------------------------------------------------------------- */



void MainWindow::afficher_enregistrer_commande() { // Afficher la fenêtre Enregistrer Commande

    // Remise à zéro de la page
    CleanMainWindow();

    // Affiche le detail de la connexion
    afficher_detail_connexion();

    /* ---- Requete libre ---- */

    // On crée un QLabel Enregistrer Une Nouvelle Vente
    titreEnregistrerCommande = new QLabel(tr("Enregistrer une nouvelle vente"), this);
    mainLayout->addWidget(titreEnregistrerCommande, 1, 0);

    // On initialise choixMagasin (QComboBox avec tous les codes postaux des différents magasins
    choixMagasin = addItemChoixMagasin();
    mainLayout->addWidget(choixMagasin, 3, 3, 1, 1);

    // On crée un QLabel id Produit
    QLabel *titreIdProduit = new QLabel(tr("id Produit :"), this);
    mainLayout->addWidget(titreIdProduit, 2, 0);

    // On crée un QSlider qui varie de 1 à 100000, on fait pointer notre variable golbale vers le QSlider
    QSpinBox* slider1 = new QSpinBox;
    slider1->setMaximum(100000);
    slider1->setMinimum(1);
    idProduitEnregistrerCommande = slider1;
    mainLayout->addWidget(idProduitEnregistrerCommande, 3, 0, 1, 1);

    // On crée un QLabel Quantité
    QLabel *titreQuantite = new QLabel(tr("Quantité :"), this);
    mainLayout->addWidget(titreQuantite, 2, 1);

    // On crée un QSpinBox qui varie de 1 à 100000, on fait pointer notre variable globale vers le QSpinBox
    QSpinBox* spin1 = new QSpinBox;
    spin1->setMaximum(100000);
    spin1->setMinimum(1);
    quantiteEnregistrerCommande = spin1;
    mainLayout->addWidget(quantiteEnregistrerCommande, 3, 1, 1, 1);

    // On crée un QLabel id Client
    QLabel *titreIdClient = new QLabel(tr("id Client :"), this);
    mainLayout->addWidget(titreIdClient, 2, 2);

    // On crée un QSpinBox qui varie de 1 à 100000, on fait pointer notre variable golbale vers le QSpinBox
    QSpinBox* spin2 = new QSpinBox;
    spin2->setMaximum(100000);
    spin2->setMinimum(1);
    idClientEnregistrerCommande = spin2;
    mainLayout->addWidget(idClientEnregistrerCommande, 3, 2, 1, 1);

    // On crée un QLabel Mode De Paiement
    QLabel *titreModePaiement = new QLabel(tr("Mode de Paiement :"), this);
    mainLayout->addWidget(titreModePaiement, 2, 3);

    // On crée un QComboBox avec Carte Bancaire et Chèque comme choix, on fait pointer notre variable globale vers
    // le QComboBox
    QComboBox* comboBox = new QComboBox;
    comboBox->addItem("Carte Bancaire");
    comboBox->addItem("Chèque");
    modePaiementEnregistrerCommande = comboBox;
    mainLayout->addWidget(modePaiementEnregistrerCommande, 3, 3, 1, 1);

    // On crée un bouton Enregistrer, on le connecte à la méthode OnClickedEnregistrerCommande
    m_pSendRequeteLibre = new QPushButton(tr("Enregistrer"), this);
    connect(m_pSendRequeteLibre, &QPushButton::clicked, this, &MainWindow::OnClickedEnregistrerCommande);
    mainLayout->addWidget(m_pSendRequeteLibre, 3, 5);

    // On montre la langue du logiciel dans le statusBar
    this->statusBar()->showMessage(statu_bar);


    menuBar()->setNativeMenuBar(false); // Permet de mettre la menu bar dans la fenetre (comme sur windows)
    this->setCentralWidget (new QWidget (this)) ;
    this->centralWidget()->setLayout(mainLayout);

}

void MainWindow::OnClickedEnregistrerCommande() { // Enregistre une commande en fonction des paramètres d'entrée

    // On cherche dans la base de donnée l'idMagasin du magasin où travaille l'employé connecté
    QSqlQuery query = RequeteSQL("SELECT id FROM magasins WHERE code_postal = " + choixMagasin->currentText());
    query.next();
    QString id_magasin = query.value(0).toString();

    /* ---- Vérification du stock avant de commander ---- */

    // On effectue une requête sur le stock de l'article à commander dans le magasin où travaille l'employé
    query = RequeteSQL("SELECT COUNT(idProduit) FROM stocks WHERE idProduit = "
                                 + idProduitEnregistrerCommande->text() + " AND idMagasin = "
                                 + id_magasin);
    query.next();

    // On récupère le résultat de la requête
    QString quantite_stock = query.value(0).toString();

    // Si il n'y a pas assez de stock pour la commande
    if (quantite_stock.toInt() < quantiteEnregistrerCommande->text().toInt()) {

        // Alors on affiche un message sous forme de QLabel indiquant que la commande ne peut être satisfaite
        QLabel *label = new QLabel;
        label->setText("Erreur. Il n'y a pas assez de stock pour satisfaire cette commande.");
        mainLayout->addWidget(label, 5, 0, 1, 5);

    }

    /* ---- Requête pour la commande ainsi que son détail ---- */

    // Sinon, si il y a assez de stock on effectue une commande avec tous les paramètres nécessaire
    if (quantite_stock.toInt() >= quantiteEnregistrerCommande->text().toInt() &&
            quantiteEnregistrerCommande->text().toInt() > 0) {

        // On crée une requête pour ajouter une nouvelle commande dans la BDD
        QString requete_commandes = "INSERT INTO `commandes` (`id`, `idClient`, `idMagasin`, `date_commande`, "
                                    "`EmployeResponsableVente`, `mode_paiement`) VALUES(";

        // On récupère la date du jour
        QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd"); // Date du jour

        // On récupère l'id de la future commande en incrémentant l'id de la dernière commande
        QString id_commande = get_autoincrement_id_commande();
        // On ajoute à la requête cet id
        requete_commandes += "'" + id_commande + "', ";

        // On ajoute à la requête l'id du client entré dans l'IHM
        requete_commandes += "'" + idClientEnregistrerCommande->text() + "', ";

        // On ajoute l'id du magasin où travaille l'employé
        requete_commandes += "'" + id_magasin + "', ";

        // On ajoute la date du jour
        requete_commandes += "'" + date + "', ";

        // On ajoute l'id de l'employé connecté à la base de données
        requete_commandes += "'" + QString::number(id_employes) + "', ";

        // On ajoute le mode de paiement
        requete_commandes += "'" + modePaiementEnregistrerCommande->currentText() + "');";


        // On crée une nouvelle requête pour le détail de la commande
        QString requete_detail_commande = "INSERT INTO `details_commande` (`idCommande`, "
                                          "`idProduit`, `quantite`) VALUES(";

        // On ajoute l'id de la commande
        requete_detail_commande += "'" + id_commande + "', ";

        // On ajoute l'id du produit commandé
        requete_detail_commande += "'" + idProduitEnregistrerCommande->text() + "', ";

        // On ajoute la quantité commandé
        requete_detail_commande += "'" + quantiteEnregistrerCommande->text() + "');";


        // On ouvre le fichier de la BDD et on lit toute la base
        QFile file("/Users/Victor/Desktop/magasin_QT_BDD.sql");
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream flux(&file);

        // On vient ajouter à la fin de ce fichier les 2 requêtes
        flux << requete_commandes << "\n" << requete_detail_commande << "\n";


        /* ---- Mise à jour du stock ---- */

        // On supprime le stock de ce produit dans ce magasin
        flux << "DELETE FROM `stocks` WHERE `idProduit` = '"
                           + idProduitEnregistrerCommande->text() + "' AND `idMagasin` = '"
                           + id_magasin + "';\n";

        // Si il y avait plus de stock que nécessaire
        if ((quantite_stock.toInt() - quantiteEnregistrerCommande->text().toInt()) > 0) {

            // On rajoute le stock restant dans la BDD à la fin du fichier
            for (int i=0; i<(quantite_stock.toInt() - quantiteEnregistrerCommande->text().toInt()); i++) {

                flux << "INSERT INTO `stocks` (`idProduit`, `idMagasin`) VALUES('"
                                   + idProduitEnregistrerCommande->text() + "', '"
                                   + id_magasin + "');\n";

            }
        }

        // On ferme le fichier
        file.close();


        /* ---- Affichage de la commande ---- */

        // On effectue une requête en cherchant le détail de la commande passé
        tableau_reponse = &TableRequeteSQL("SELECT c.id, a.nom, p.nom, d.quantite, c.date_commande, "
                                               "c.mode_paiement FROM commandes c "
                                               "INNER JOIN details_commande d INNER JOIN clients a INNER "
                                               "JOIN produits p ON c.id = " + id_commande +
                                               " AND d.idCommande = c.id AND a.id = c.idClient AND p.id = "
                                               "d.idProduit");

        // On crée un QLabel pour afficher que la commande a bien été enregistré et qu'il reste, ou pas, du stock
        QLabel *label = new QLabel;
        label->setText("Votre commande est bien enregistré. Il reste " +
                       QString::number(quantite_stock.toInt() - quantiteEnregistrerCommande->text().toInt()) +
                       " article(s) en stock.");

        // Si le stock est vide on l'indique dans le QLabel
        if (quantite_stock.toInt() == quantiteEnregistrerCommande->text().toInt())
            label->setText("Votre commande est bien enregistré. "
                           "Attention : Le stock est désormais vide pour ce produit.");

        // On ajoute les informations à la fenêtre
        mainLayout->addWidget(label, 5, 0, 1, 5);
        mainLayout->addWidget(tableau_reponse, 6, 0, 1, 5);
    }

}

QString MainWindow::get_autoincrement_id_commande() { // On récupère l'id de la future commande

    // On effectue une requête pour savoir la valeur max de l'id commande
    QSqlQuery query;
    query = RequeteSQL("SELECT MAX(id) FROM commandes");
    query.next();

    // On incrémente cet id de 1
    int id = query.value(0).toString().toInt();
    id++;

    // On retourne la valeur sous forme de QString
    return QString::number(id);

}



/* -----------------------------------------------------------------------------------
 *
 *                                  RECHERCHER MAGASIN
 *
 * --------------------------------------------------------------------------------- */



void MainWindow::afficher_rechercher_magasin() { // Affiche la fenêtre Rechercher Dans Le Magasin

    // Remise à zéro de la page
    CleanMainWindow();

    // Affiche le detail de la connexion
    afficher_detail_connexion();

     /*---- Recherche dans magasin ---- */

    // Création d'un QLabel Rechercher Dans Le Magasin
    titreRechercheMagasin = new QLabel(tr("Rechercher dans le magasin"), this);
    mainLayout->addWidget(titreRechercheMagasin, 1, 0);

    // Création d'un QLabel Nom Ou Id Du Produit
    QLabel *titreLineEditRechercheMagasin = new QLabel(tr("Nom ou Id du produit :"), this);
    mainLayout->addWidget(titreLineEditRechercheMagasin, 2, 0);

    // Création d'un Qlabel Code Postale Magasin
    QLabel *titreCheckBoxRechercheMagasin = new QLabel(tr("Code Postale Magasin :"), this);
    mainLayout->addWidget(titreCheckBoxRechercheMagasin, 2, 1);

    // Création d'un QLineEdit, le QLineEdit globale pointera dessus
    QLineEdit* lineEdit1 = new QLineEdit;
    lineEdit1->setText("");
    lineEditRechercheMagasin = lineEdit1;
    mainLayout->addWidget(lineEditRechercheMagasin, 3, 0, 1, 1);

    // Le QComboBox choixMagasin pointera vers un QComboBox créée à l'aide de la méthode addItemChoixMagasin()
    choixMagasin = addItemChoixMagasin();
    mainLayout->addWidget(choixMagasin, 3, 1, 1, 1);

    // Création d'un boutton Rechercher connecté à la méthode OnClickedRechercheMagasin
    m_pSendRechercheMagasin = new QPushButton(tr("Rechercher"), this);
    connect(m_pSendRechercheMagasin, &QPushButton::clicked, this,&MainWindow::OnClickedRechercheMagasin);
    mainLayout->addWidget(m_pSendRechercheMagasin, 3, 2, 1, 1);

    // On montre la langue du logiciel dans le statusBar
    this->statusBar()->showMessage(statu_bar);


    menuBar()->setNativeMenuBar(false); // Permet de mettre la menu bar dans la fenetre (comme sur windows)
    this->setCentralWidget (new QWidget (this)) ;
    this->centralWidget()->setLayout(mainLayout);

}

QComboBox* MainWindow::addItemChoixMagasin() { // Retourne un QComboBox remplie des code postaux des différents magasin

    // On fait une requête pour avoir le code postal du magasin où travaille l'employé connecté
    QSqlQuery query = RequeteSQL("SELECT m.code_postal FROM employes e INNER JOIN magasins "
                                 "m ON e.id_magasin = m.id WHERE e.id = '"
                                 + QString::number(id_employes) + "'" );

    // On regarde la première ligne de la réponse à la requête
    query.next();

    // On crée un QComboBox et on y ajoute le code postal en premier
    QComboBox * comboBox = new QComboBox;
    comboBox->addItem(query.value(0).toString());

    // On effectue une nouvelle requête pour sélectionner tous les code postaux de tous les magasins
    query = RequeteSQL("SELECT code_postal FROM magasins ORDER BY code_postal");

    // On lis une par une les lignes de la réponse à la requête
    while(query.next())
    {
        // On lis colonne par colonne de la ligne de la réponse à la requête
        for (int x=0; x < query.record().count(); ++x)
        {
            // On ajoute les code postaux au QComboBox
            comboBox->addItem(query.value(x).toString());
        }
    }

    // On retourne le QComboBox
    return comboBox;

}

void MainWindow::OnClickedRechercheMagasin() { // Effectue la requête et l'affiche dans la fenêtre

    // On crée un QTableWidget

    // On effectue une requête et on met la réponse dans le QTableWidget
    tableau_reponse = &TableRequeteSQL("SELECT s.idProduit, p.nom, p.description, m.code_postal "
                                   "FROM stocks s INNER JOIN magasins m INNER JOIN produits p "
                                   "WHERE (p.nom LIKE '%" + lineEditRechercheMagasin->text() + "%' "
                                   "OR p.id = '" + lineEditRechercheMagasin->text() + "') AND s.idProduit = p.id "
                                   "AND m.code_postal = " + choixMagasin->currentText() +
                                   " AND s.idMagasin = m.id AND p.id = s.idProduit ORDER BY p.nom");

    // On ajoute le QtableWidget dans la fenêtre
    mainLayout->addWidget(tableau_reponse, 4, 0, 1, 0);

}



/* -----------------------------------------------------------------------------------
 *
 *                                  REQUÊTES SQL
 *
 * --------------------------------------------------------------------------------- */




QSqlQuery MainWindow::RequeteSQL(QString requete) { // Effectue une requête et retourne le résulat

    if (db.open()) { // On ouvre la BDD

        // On exécute la BDD
        executer_BDD(); // Chelou !!!

        // On crée QSqlQuery avec la requête entré en paramètre
        QSqlQuery query;
        // Si la requête abouti on retourne le résultat
        if(query.exec(requete))
        {
            return query;
        }
        else // Sinon on affiche une erreur
        {
            std::cout << q2c(tr("Une erreur s'est produite. Erreur #2")) << std::endl
                      << q2c(query.lastError().text()) << std::endl;
            close();
        }
    }
    else {
        std::cout << q2c(tr("Une erreur s'est produite. Erreur #3")) << std::endl;
        close();
    }
}

QTableWidget &MainWindow::TableRequeteSQL(QString requete) { // Effectue une requête et retourne le résultat sous forme
    // de QTableWidget

    bool AfficherColonne = false;

    //std::cout << requete << std::endl;

    // On crée un QTableWidget
    QTableWidget *tableWidget = new QTableWidget;

    // On initialise la table à 1 ligne et 1 colonne
    tableWidget->setRowCount(1);
    tableWidget->setColumnCount(1);

    // Si la BDD s'ouvre
    if (db.open()) {

        // On exécute la BDD
        executer_BDD(); // Chelou !!!

        // On crée une QSqlQuery avec la requête entré en paramètre
        QSqlQuery query;
        // Si la requête aboutie
        if(query.exec(requete)) {
            // À chaque colonne du résultat de la reqûete, on défini le nom de la colonne
            tableWidget->setColumnCount(query.record().count());
            int i=0;

            // On lit chaque ligne de réponse à la requête
            while(query.next()) {

               // Si les noms des colonnes n'a pas était déjà défini
               if (!AfficherColonne) {
                   // On défini chaque colonnes avec le nom des colonnes de la réponse
                   for (int x=0; x < query.record().count(); ++x) {
                       tableWidget->setHorizontalHeaderItem(x, new QTableWidgetItem(query.record().fieldName(x)));
                   }

                   AfficherColonne = true;
               }
               else { // Sinon on ajoute une ligne au tableau
                    tableWidget->setRowCount(i+1);
               }

               // Pour chaque colonne on ajoute la réponse au tableau
               for (int x=0; x < query.record().count(); ++x) {
                    tableWidget->setItem(i, x, new QTableWidgetItem(query.value(x).toString()));
               }
               i++;
            }
        }
        else {
            // Si la requête n'est pas bonne on l'affiche dans la première case du tableau
            tableWidget->setItem(0, 0, new QTableWidgetItem(tr("Une erreur s'est produite. "
                                                            "Erreur #2\n") + query.lastError().text()));
        }
    }
    else {
        // Si la BDD ne s'ouvre pas on affiche une erreur dans le tableau
        tableWidget->setItem(0, 0, new QTableWidgetItem(tr("Une erreur s'est produite. Erreur #3\n")));
    }

    // On retourne le tableau
    return *tableWidget;
}


/* -----------------------------------------------------------------------------------
 *
 *                                  CHARGER, EXÉCUTER LA BDD
 *
 * --------------------------------------------------------------------------------- */


QString MainWindow::charger_BDD(QString lieu) { // Charge le fichier de la BDD et retourne l'intégralité du fichier


    QFile file(lieu);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&file);
    QString requete = flux.readAll();

    file.close();

    return requete;

}

void MainWindow::executer_BDD() { // Execute toutes les requêtes de la BDD

    // On charge la BDD, on obtient toutes les requêtes que l'on split tous les ';'
    QStringList split = charger_BDD("/Users/Victor/Desktop/magasin_QT_BDD.sql").split(";");
    int i=0;

    // Pour chaque item du split on execute la requête
    while (i < split.size()) {
        QSqlQuery query;
        query.exec(split.at(i).toLocal8Bit().constData());
        i++;
    }

}




MainWindow::~MainWindow() // Destructeur
{
    // On ferme la BDD
    db.close();
}
