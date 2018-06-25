#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QtSql>
#include <iostream>
#include <QString>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QGridLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <time.h>
#include <stdlib.h>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTableView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSlider>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QObject>
#include <QSpinBox>
#include <QStatusBar>
#include <QTranslator>

#define q2c(string) string.toStdString()

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    /* ---- Général ---- */

    QString *BDD;
    QGridLayout *mainLayout;
    int id_employes;
    QSqlDatabase db;
    QTableWidget *tableau_reponse = new QTableWidget;
    QTranslator translator;
    QString statu_bar = "fr_FR";

    /* ---- Connexion ---- */

    QLineEdit *m_pMotDePasse;
    QLineEdit *m_pMail;
    QPushButton *m_pLogInButton;
    QLabel *titreMail;
    QLabel *titreMotDePasse;
    bool connecter = false;

    /* ---- Menu, toolBar, details connexion ---- */

    QLabel *DetailConnexion;
    QAction *requete_libre;
    QAction *recherche_table;
    QAction *enregistrer_commande;
    QAction *recherche_magasin;
    QMenu *fichier;
    QMenu *edition;
    QMenu *aide;
    QMenu *langue;

    /* ---- Requête Libre ---- */

    QLabel *titreRequeteLibre;
    QTextEdit *m_pRequeteLibre;
    QPushButton *m_pSendRequeteLibre;
    QPushButton *m_pSELECTRequeteLibre;
    QPushButton *m_pWHERERequeteLibre;
    QPushButton *m_pFROMRequeteLibre;
    QPushButton *m_pDELETERequeteLibre;
    QTextEdit *m_pResultatRequeteLibre;

    /* ---- Recherche dans la BDD ---- */

    QPushButton *m_pSendRechercheBDD;
    QComboBox *choixBDD = new QComboBox;
    QStringList strListRechercheBDD;
    QListWidget *checkedBox = new QListWidget;
    QStringList listTypeSQL;
    QSlider *slider1RechercheBDD = new QSlider;
    bool boolSlider1 = false;
    QLabel *label1RechercheBDD = new QLabel;
    QLineEdit *lineEdit1RechercheBDD = new QLineEdit;
    bool boolLineEdit1 = false;
    QLabel *label2RechercheBDD = new QLabel;
    QLabel *labelTitreFiltreRechercheBDD = new QLabel;
    QPushButton *buttonFiltreRechercheBDD;

    /* ---- Rechercher dans le magasin ---- */

    QLabel *titreRechercheMagasin;
    QLineEdit *lineEditRechercheMagasin = new QLineEdit;
    QComboBox *choixMagasin = new QComboBox;
    QPushButton *m_pSendRechercheMagasin;

    /* ---- Enregistrer nouvelle commande ---- */

    QLabel *titreEnregistrerCommande;
    QSpinBox *idProduitEnregistrerCommande = new QSpinBox;
    QSpinBox *quantiteEnregistrerCommande = new QSpinBox;
    QSpinBox *idClientEnregistrerCommande = new QSpinBox;
    QComboBox *modePaiementEnregistrerCommande = new QComboBox;


public slots:
    void chargerRequete();
    void sauvegarderRequete();
    void afficherContenu();
    void afficherAProposDe();
    void changeLangueFR();
    void changeLangueEN();
    void afficher_requete_libre();
    void afficher_rechercher_BDD();
    void afficher_enregistrer_commande();
    void afficher_rechercher_magasin();
    void ChangedIndexRechercheBDD();
    void ChangedSliderRechercheBDD(int value);

public:
    MainWindow(QWidget *parent = 0);
    void connexion();
    QString charger_BDD(QString);
    void executer_BDD();
    void OnClickedLogIn();
    void ShowMenuBar();
    void ShowMainWindow();
    void DeleteLogInWindow();
    void CleanMainWindow();
    QSqlQuery RequeteSQL(QString);
    QTableWidget &TableRequeteSQL(QString);
    void afficher_detail_connexion();
    void OnClickedRequeteLibre();
    void OnClickedButtonRequeteLibre();
    void OnClickedButtonRechercheBDD();
    void SetSliderRechercheBDD(int min, int max, QString);
    QStringList ReturnTableSQL(QString);
    void OnClickedFiltreRechercheBDD();
    QComboBox *addItemChoixMagasin();
    void OnClickedRechercheMagasin();
    void OnClickedEnregistrerCommande();
    QString get_autoincrement_id_commande();
    ~MainWindow();
};

#endif // MAINWINDOW_H
