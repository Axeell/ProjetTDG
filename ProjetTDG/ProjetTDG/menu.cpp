#include "grman/grman.h"
#include <iostream>
#include "Menu.h"


#include <allegro.h>
#include <time.h>
#include <math.h>



#include "graph.h"

using namespace std;
void menu (BITMAP* ecran, BITMAP* Menu,BITMAP* Jouer, BITMAP* Commandes, BITMAP* Quitter, int v, Graph a, Graph b, Graph c)
{

    if (v==0)
    {
        while (v==0)
        {

            blit(Menu,ecran,0,0,0,0,800,600);

            if (mouse_x>=290 && mouse_x<=525 && mouse_y>=80 && mouse_y<=160)
            {
                blit(Jouer,ecran,0,0,0,0,800,600);
                if (mouse_b==1)
                {
                    v=1;
                    rest(300);
                }
            }


            if (mouse_x>=290 && mouse_x<=525 && mouse_y>=190 && mouse_y<=265)
            {
                blit(Commandes,ecran,0,0,0,0,800,600);
                if (mouse_b==1)
                {
                    v=2;
                    rest(150);
                }
            }

            if (v==2)
            {

                v=0;
                cout << "règles "<< endl;

            }


            if (mouse_x>=290 && mouse_x<=525 && mouse_y>=300 && mouse_y<=380)
            {
                blit(Quitter,ecran,0,0,0,0,800,600);
                if (mouse_b==1)
                {
                    v=3;
                    rest(150);
                }
            }

            blit(ecran,screen,0,0,0,0,800,600);
            clear(ecran);
        }

        if (v==1)
        {
            BITMAP* Menubis=load_bitmap("Menubis.bmp",NULL); ///Déclaration des bitmaps du menu
            BITMAP* Menubis1=load_bitmap("Menubis1.bmp",NULL);
            BITMAP* Menubis2=load_bitmap("Menubis2.bmp",NULL);
            BITMAP* Menubis3=load_bitmap("Menubis3.bmp",NULL);
            BITMAP* Retour=load_bitmap("Retour.bmp",NULL);
            menubis(ecran,Menubis,Menubis1,Menubis2,Menubis3,Retour,Menu,Jouer,Commandes,Quitter, a, b, c);

        }


        if (v==3)
        {

            allegro_exit();
            exit(EXIT_FAILURE);

        }
    }
}

void menubis (BITMAP* ecran, BITMAP* Menubis, BITMAP *Menubis1, BITMAP *Menubis2, BITMAP *Menubis3,BITMAP* Retour, BITMAP* Menu,BITMAP* Jouer, BITMAP* Commandes, BITMAP* Quitter, Graph a, Graph b, Graph c)
{

    bool bol= true;
    int x=0;
    int v=0;
    int choix;

    while (bol)
    {

        blit(Menubis,ecran,0,0,0,0,800,600);

        if (mouse_x>=290 && mouse_x<=525 && mouse_y>=80 && mouse_y<=160)
        {
            blit(Menubis1, ecran, 0,0,0,0,800,600);
            if(mouse_b==1)
            {
                x=1;
                choix=1;
                a.make_example();

            }

        }

        if (mouse_x>=290 && mouse_x<=525 && mouse_y>=190 && mouse_y<=265)
        {
            blit(Menubis2,ecran, 0,0,0,0,800,600);
            if (mouse_b==1)
            {
                x=1;
                choix=2;
                b.make_example();
            }
        }

        if (mouse_x>=290 && mouse_x<=525 && mouse_y>=300 && mouse_y<=380)
        {
            blit(Menubis3,ecran, 0,0,0,0,800,600);
            if (mouse_b==1)
            {
                x=1;
                choix=3;
                c.make_example();
            }
        }

        if (mouse_x>=33 && mouse_x<=94 && mouse_y>=530  && mouse_y<=580)
        {
            blit (Retour,ecran, 0,0,0,0,800,600);
            if (mouse_b==1)
            {
                menu(ecran,Menu,Jouer,Commandes,Quitter,v, a, b, c);
            }
        }

    }

}
/*void creationGraphe(int choix)
{
    string nom;
    Graph graphe;
    switch (choix)
    {
       case 1: nom= "....."
       break;
       case 2: nom= "...."
       break;
       case 3: nom= "...."
       break;

       default :
        break;
    }

    graphe.make_example(nom);

     /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }


}*/
