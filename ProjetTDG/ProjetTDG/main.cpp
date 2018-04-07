#include "grman/grman.h"
#include <iostream>
#include "Menu.h"
#include "graph.h"
#include <time.h>
#include <math.h>

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");



    /// Un exemple de graphe
    Graph graphik;
    Graph hgraphik;
    Graph igraphik;
   // graphik.make_example();

    ///DECLARATION DES BITMAPS
    BITMAP* ecran=create_bitmap(1600,1200); ///Declaration des bitmap affichage
    BITMAP* Menu=load_bitmap("Menu.bmp",NULL); ///Declaration des bitmaps du menu
    BITMAP* Jouer=load_bitmap("Jouer.bmp",NULL);
    BITMAP* Commandes=load_bitmap("Commandes.bmp",NULL);
    BITMAP* Quitter=load_bitmap("Quitter.bmp",NULL);

    int v=0; /// Variable pour le menu




    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
        menu (ecran,Menu,Jouer,Commandes,Quitter,v, graphik, hgraphik, igraphik);

        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        graphik.update();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


