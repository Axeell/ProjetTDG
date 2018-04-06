#include "graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stack>
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);



    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    m_top_box.add_child( m_sup_button );
    m_sup_button.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_sup_button.set_dim(30,30);
    m_sup_button.set_bg_color(ROUGE);
    m_sup_button.set_message("X");

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );

}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_tool_box.add_child(m_pause);
    m_pause.set_dim(80,80);
    m_pause.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_pause.set_bg_color(NOIR);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
/*void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
    add_interfaced_vertex(8,  100.0, 500, 300, "bad_clowns_xx3xx.jpg");
    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    add_interfaced_edge(0, 1, 2, 50.0);
    add_interfaced_edge(1, 0, 1, 50.0);
    add_interfaced_edge(2, 1, 3, 75.0);
    add_interfaced_edge(3, 4, 1, 25.0);
    add_interfaced_edge(4, 6, 3, 25.0);
    add_interfaced_edge(5, 7, 3, 25.0);
    add_interfaced_edge(6, 3, 4, 0.0);
    add_interfaced_edge(7, 2, 0, 100.0);
    add_interfaced_edge(8, 5, 2, 20.0);
    add_interfaced_edge(9, 3, 7, 80.0);
}*/
void Graph::first_graph()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    std::string num_sommet, poids, posx, posy, img;
    double r, K;
    std::ifstream monPremierGraph("monPremierGraph_som.txt", std::ifstream::in);
    while ( monPremierGraph >> num_sommet)
    {
        monPremierGraph >> poids >> posx >> posy >> r >> K >> img;
        int inum_sommet, ipoids, iposx, iposy;
        std::stringstream ssnum_sommet(num_sommet);
        ssnum_sommet >> inum_sommet;
        std::stringstream sspoids(poids);
        sspoids >> ipoids;
        std::stringstream ssposx(posx);
        ssposx >> iposx;
        std::stringstream ssposy(posy);
        ssposy >> iposy;
        add_interfaced_vertex(inum_sommet, ipoids, iposx, iposy, r, K, img);
    }
    monPremierGraph.close();
    std::string som1, som2, num_arete;
    std::ifstream monPremierGraph_ar("monPremierGraph_ar.txt", std::ifstream::in);
    while ( monPremierGraph_ar >> num_arete)
    {
        monPremierGraph_ar >> som1 >> som2 >> poids;
        int inum_arete, ipoids, isom1, isom2;
        std::stringstream ssnum_arete(num_arete);
        ssnum_arete >> inum_arete;
        std::stringstream sspoids(poids);
        sspoids >> ipoids;
        std::stringstream sssom1(som1);
        sssom1 >> isom1;
        std::stringstream sssom2(som2);
        sssom2 >> isom2;
        add_interfaced_edge(inum_arete, isom1, isom2, ipoids);
    }
    monPremierGraph_ar.close();
}
void Graph::save_first_graph()
{
    std::ofstream fichier("monPremierGraph_som.txt", std::ios::out | std::ios::trunc);
    for (auto &e : m_vertices)
    {
        fichier << e.first << " " << e.second.m_value << " " << e.second.m_interface->m_top_box.get_posx() << " " << e.second.m_interface->m_top_box.get_posy() << " " << e.second.m_r << " " << e.second.m_K << " " << e.second.m_img << std::endl;
    }
    fichier.close();
    std::ofstream fichier_ar("monPremierGraph_ar.txt", std::ios::out | std::ios::trunc);
    for (auto &e : m_edges)
    {
        std::cout << e.first << " " << e.second.m_from<< " " << e.second.m_to << " " << e.second.m_weight << std::endl;
        fichier_ar << e.first << " " << e.second.m_from << " " << e.second.m_to << " " << e.second.m_weight << std::endl;
    }
    fichier_ar.close();

}


/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();
    if(m_interface->m_pause.clicked())
    {
        int sommet_de_depart, sommet_d_arrive;
        std::cout << "sommet de depart: ";
        std::cin >> sommet_de_depart;
        std::cout << "sommet d'arrivee: ";
        std::cin >> sommet_d_arrive;
        ajouter_arete(sommet_de_depart, sommet_d_arrive);
    }

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, double r, double K, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi, pic_name, K, r, idx);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    /* std::map<int, Vertex>::iterator it_vertex_predator = m_vertices.find(id_vert1);
     it_vertex_predator->second.m_out.push_back(idx);

     std::map<int, Vertex>::iterator it_vertex_prey = m_vertices.find(id_vert2);
     it_vertex_prey->second.m_in.push_back(idx);*/
    m_edges[idx].m_from = id_vert1;

    m_edges[idx].m_to = id_vert2;



    m_vertices[id_vert1].m_out.push_back(idx);

    m_vertices[id_vert2].m_in.push_back(idx);

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei, id_vert1, id_vert2);
}

void Graph::evolution_population()
{
    ///std::cout << "'" << std::endl;
    std::map<int, double> backup_population;
    for(std::map<int, Vertex>::iterator it=m_vertices.begin(); it!=m_vertices.end(); ++it)
    {
        ///  std::cout << it->second.m_value << std::endl;
        backup_population.insert(std::pair<int, double>(it->first, it->second.m_value));
    }

    ///calcul contrib totales especes proies et predatrices de chaque vertex
    for(std::map<int, Vertex>::iterator it=m_vertices.begin(); it!=m_vertices.end(); ++it)
    {
        double contrib_predator = 0;
        for(std::vector<int>::iterator it_predator = it->second.m_in.begin(); it_predator != it->second.m_in.end(); ++it_predator)
        {
            std::map<int, Edge>::iterator it_edge_predator = m_edges.find(*it_predator);
            double weight = it_edge_predator->second.m_weight;
            int predator = it_edge_predator->second.m_to;

            std::map<int, double>::iterator it_population_predator = backup_population.find(predator);
            double population_predator = it_population_predator->second;
            ///std::cout << it_edge_predator->first << " " << population_predator << " ";
            contrib_predator += weight * population_predator / 5000;
        }

        double contrib_prey = 0;
        for(std::vector<int>::iterator it_prey = it->second.m_out.begin(); it_prey != it->second.m_out.end(); ++it_prey)
            ///for(int i = 0; i<it->second.m_out.size(); ++i)
        {
            int i = *it_prey;
            int i2 = it->first;
            std::map<int, Edge>::iterator it_edge_prey = m_edges.find(i);
            double weight = it_edge_prey->second.m_weight;
            int prey = it_edge_prey->second.m_to;

            std::map<int, double>::iterator it_population_prey = backup_population.find(prey);
            double population_prey = it_population_prey->second;
            std::cout << it_edge_prey->first << " " << population_prey << " ";
            contrib_prey += weight * population_prey / 5000;
        }
        std::cout << std::endl;
        std::map<int, double>::iterator it_current_specie = backup_population.find(it->first);
        double current_population = it_current_specie->second;
        double new_population = current_population + it->second.m_r * current_population * (1. - current_population / it->second.m_K) - contrib_predator + contrib_prey;

        if (new_population < 0.0)
            new_population = 0.0;

        it->second.m_value = new_population;
    }
    std::cout << "'";
    std::cout << std::endl;
    m_current_time +=1.;
    backup_population.clear();
}
void Graph::supprimer_un_sommet(int indice)
{
    for(int i = 0; i<=m_vertices.find(indice)->second.m_out.size(); ++i)
    {
        supprimer_une_arete(m_vertices.find(indice)->second.m_out[i]);
    }
    for(int i = 0; i<=m_vertices.find(indice)->second.m_in.size(); ++i)
    {
        supprimer_une_arete(m_vertices.find(indice)->second.m_in[i]);
    }

    Vertex &remed=m_vertices.at(indice);
    if (m_interface && remed.m_interface)

    {
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_box );

    }

    m_vertices.erase( indice );

}
void Graph::supprimer_une_arete(int eidx)
{

    /// référence vers le Edge à enlever

    Edge &remed=m_edges.at(eidx);
    std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;
    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

    std::cout << m_edges.size() << std::endl;



    /// test : on a bien des éléments interfacés

    if (m_interface && remed.m_interface)

    {

        /// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :

        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */

        /* m_interface->m_main_box.add_child(ei->m_top_edge);  */

        /* m_edges[idx] = Edge(weight, ei); */

        /// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr

        /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

        /// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe

        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );

    }



    /// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !

    /// References sur les listes de edges des sommets from et to

    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;

    std::vector<int> &veto = m_vertices[remed.m_to].m_in;

    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );

    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );



    /// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)

    /// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface

    /// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !

    m_edges.erase( eidx );

    /// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2

    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;

    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;

    std::cout << m_edges.size() << std::endl;

}
void Graph::ajouter_sommet(double r, double K, std::string pic_name)
{
    int idx = -1;
    for (const auto &e : m_vertices)
    {
        if(e.first>idx)
            idx = e.first;
    }
    idx++;
    add_interfaced_vertex(idx, 0, 600, 600, r, K, pic_name);
}
void Graph::ajouter_arete(int som_1, int som_2)
{
    int idx = 0;
    for (const auto &e : m_edges)
    {
        if(e.first>idx)
            idx = e.first;
    }
    idx++;
    add_interfaced_edge(idx, som_1, som_2, 0);
}

void Graph::composantes_fortement_connexes()
{
    /* std::stack<int>parcours;
     bool tab[m_vertices.size()] = {false};
     int i = 0;
     int b, a;
     m_vertices[0].m_marque = true;
     std::vector<int>index;
     std::vector<int>noeud;
     while(!tab[i])         ///
     {
         tab[i] = true;
         m_vertices.find(i)->second.m_marque = true;
         std::cout << m_vertices.find(i)->first;
         bool marquer = false;
         a = 0;

         while(!marquer)
         {
             ///b =  m_vertices.find(i)->second.m_out[a];
             int out1 = m_vertices.find(i)->second.m_out[a];
             int v =  m_edges.find(out1)->second.m_to;
             ///int som_out = m_edges.find(out1)->second.m_to.m_indice;
             if(!tab[v])
             {
                 ///  int out = m_vertices.find(i)->second.m_out[1];
                 if ( m_vertices.find(i)->second.m_out.size() > 1)
                     noeud.push_back(i);
                 index.push_back(i);
                 parcours.push(i);
                 i = m_vertices.find(i)->second.m_out[a];
                 i = m_edges.find(i)->second.m_to;
                 // tab[v] = true;
                 marquer = true;

             }
             else if( a+1 < m_vertices.find(i)->second.m_out.size())                 ///
             {
                 a++;
             }
             else
             {
                 ///i = index[index.size()-1];
                 index[i] = v;
                 int index_remont = i;
                 while (parcours.top() != v)
                 {
                     index[parcours.top()] = v;
                     parcours.pop();
                 }
                 for(int j = 0; j<=index.size(); ++j)
                 {
                     std::cout << index[j] << "f ";
                 }

                 marquer = true;
             }
         }
     }
    ///m_vertices.find(i)->second.m_marque = true;

    */




}

int * Graph::uneComposanteFortementConnexe ()
{
//Variables locales
   /// int** adjacence;
    int ordre;
    int idx = 0;
    for (const auto &e : m_vertices)
    {
        if(e.first>idx)
            idx = e.first;
    }
    idx++;
    ordre = idx;
    int ordre_arete;
    int idx_arete = 0;
    for (const auto &e_ar : m_edges)
    {
        if(e_ar.first>idx_arete)
            idx_arete = e_ar.first;
    }
    idx_arete++;
    ordre_arete = idx_arete;
    int s = 0;
   /* int *c1, *c2 ; // composantes connexes directes partant de s et indirectes arrivant vers s
    int *c ; // composante fortement connexe = c1  c2 à retourner
    int *marques ; // tableau dynamique indiquant si les sommets sont marqués ou non*/
    int x, y ; // numéros de sommets intermédiaires des composantes connexes
    int ajoute = 1 ; // booléen indiquant si une nouvelle composante connexe est ajoutée
// Allouer les tableaux dynamiques c1, c2, c et marques de taille « ordre »
    int c1[ordre] = {0};
    int c2[ordre] = {0};
    int c[ordre];
    bool marques[ordre] = {false};
    std::vector<std::vector<bool>>adjacence;
    for(int i = 0; i< ordre; i++)
    {
        std::vector<bool>buffer;
        for(int j = 0; j<ordre; j++)
        {
            buffer.push_back(false);
        }
        adjacence.push_back(buffer);
    }


    for(std::map<int, Edge>::iterator it=m_edges.begin(); it!=m_edges.end(); ++it)
    {
        ///std::vector<bool>buffer;
       int from = it->second.m_from;
       int to = it->second.m_to;
       adjacence[from][to]  = true;
    }



// Initialiser les valeurs de ces tableaux à 0

// Rendre le sommet s connexe
    c1[s] = 1 ;
    c2[s] = 1 ;
// Recherche des composantes connexes partant de s à ajouter dans c1 :
    while (ajoute)
    {
        ajoute = 0; // à chaque tour, recherche d’une nouvelle composante connexe à ajouter
// Pour tous les sommets x non marqués et connectés en partant de s
// Marquer chaque sommet x et connecter les sommets non marqués y adjacents à x
        for (x=0 ; x<ordre ; x++)
        {
            if (!marques[x] && c1[x])
            {
                marques[x] = 1 ;
                for (y=0 ; y<ordre ; y++)
                {
                    if (adjacence[x][y] && !marques[y])
                    {
                        c1[y] = 1 ;
                        ajoute = 1 ; // nouvelle composante connexe ajoutée
                    }
                }
            }
        }
    }
    ajoute = 1;
    while (ajoute)
    {
    for (x=0 ; x<ordre ; x++)
        {
        if (!marques[x] && c2[x])
            {
                marques[x] = 1 ;
                for (y=0 ; y<ordre ; y++)
                {
                    if (adjacence[y][x] && !marques[y])
                    {
                        c2[y] = 1 ;
                        ajoute = 1 ; // nouvelle composante connexe ajoutée
                    }
                }
            }
        }
    }
// Recherche des composantes connexes arrivant à s à ajouter dans c2 :

// Composante fortement connexe c = intersection de c1 et c2
        for (x=0 ; x<ordre ; x++)
            c[x] = c1[x] & c2[x] ;
// Retourner la composante fortement connexe c
    x = 0;
    while(c[x])
    {
        std::cout << c[x] << " ";
        ++x;
    }
        return c ;

}

    void Vertex::test_click()
    {
        /* if(m_sup_button.WidgetCheckBox::interact_focus())
         {
             m_sup_button.set_bg_color(NOIR);
         }*/
    }

    Vertex Graph::get_vertex(int indice)
    {
        for(std::map<int, Vertex>::iterator it=m_vertices.begin(); it!=m_vertices.end(); ++it)
        {
            if (it->first == indice)
            {
                /// return it->second;
                std::cout << std::endl << it->first << it->second.m_value << std::endl << std::endl;

            }

        }
    }

    std::vector<int> Vertex::sommets_out()
    {
        /* for(int i = 0; i<m_out.size(); i++)
         {
             std::cout << "s"<< m_out[i] << std::endl;
         }*/
    }



