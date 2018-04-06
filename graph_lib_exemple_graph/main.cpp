#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;
    //g.make_example();
    g.first_graph();
        for(std::map<int, Vertex>::iterator it=g.m_vertices.begin(); it!=g.m_vertices.end(); ++it)
    {
        std::cout << it->second.get_value() << std::endl;
    }
    ///g.uneComposanteFortementConnexe();
    ///g.composantes_fortement_connexes();
    ///g.composantes_fortement_connexes();
    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés)
    int boucle = 0;
    grman::afficher_date();
    while ( !key[KEY_ESC] )
    {
   /*   int choix;
        std::cin >> choix;
        switch(choix)
        {
        case 1:
            std::cout << "sommet à supprimer: ";
            int sommet_a_supprimer;
            std::cin >> sommet_a_supprimer;
            g.supprimer_un_sommet(sommet_a_supprimer);
            break;
        case 2:
            std::cout << "arete à supprimer: ";
            int arete_a_supprimer;
            std::cin >> arete_a_supprimer;
            g.supprimer_une_arete(arete_a_supprimer);
            break;
        case 3:
            int sommet_de_depart, sommet_d_arrive;
            std::cout << "sommet de depart: ";
            std::cin >> sommet_de_depart;
            std::cout << "sommet d'arrivee: ";
            std::cin >> sommet_d_arrive;
            g.ajouter_arete(sommet_de_depart, sommet_d_arrive);
            break;
        }*/

        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();

       /// g.get_vertex(2);
        if (boucle == 2)
        {
            g.evolution_population();
            boucle = 0;
        }
        ++boucle;


    }
    g.save_first_graph();
    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


