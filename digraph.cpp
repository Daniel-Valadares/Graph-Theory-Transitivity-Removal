/*
* @brief Programa que implementa um algoritmo de redução de de transitividade, 
    dado um determinado grafo direcionado semi-fortemente ou fortemente conexo
* @authors Daniel Valadares de Souza Felix, Felipe Augusto Maciel Constantino, 
    Gustavo Silvestre Almeida Conceição e Larissa Valadares Silqueira
* @version 0.0.1
*/

////////////////////////////////
// Bibliotecas
////////////////////////////////

#include <bits/stdc++.h>

////////////////////////////////
// Definicoes
////////////////////////////////

using namespace std;

////////////////////////////////
// Algoritmo
////////////////////////////////

class Digraph {
    private: // Dados e Metodos privados da classe
        bool** adjMatrix; // Matriz fr adjacencia
        int V; // Numero de vertices
        int E; // Numero de arestas

        // Metodo auxiliar para removeCycle()
        bool removeCycleUtil(int u, bool visited[], bool* recStack, bool* flag) {
            if (!visited[u]) { // testar se ja visitado
                // Definir Vertice como visitado e marca como parte do stack
                visited[u] = true;
                recStack[u] = true;
                for (int v = 0; v < V; v++) {
                    v = v == u ? v + 1 : v; // Linha para nao remover loops e evitar de conflitar com a
                    if (adjMatrix[u][v]) {
                        if (!visited[v] 
                        && removeCycleUtil(v, visited, recStack, flag))
                        {
                            if (!*flag) {
                                removeEdge(u, v);
                                *flag = true;
                            }
                            return true;
                        }
                        else if (recStack[v]) 
                        {
                            if (!*flag) {
                                removeEdge(u, v);
                                *flag = true;
                            }
                            return true;  
                        } 
                    }    
                }
            }
            // Remove vertice da pilha caso ja tenha sido visitado
            recStack[u] = false;
            return false;
        }

    public: // Dados e Metodos públicos da classe
        // Constructor
        Digraph(int numVertices) {
            this->V = numVertices; // definindo numero de vertices
            this->E = 0; // definindo numero de arestas
            adjMatrix = new bool*[V]; // Alocar espaço para primeira coluna
            for (int u = 0; u < V; u++)
            {
                adjMatrix[u] = new bool[V]; // Alocar espaço para cada linha
                for (int v = 0; v < V; v++)
                    adjMatrix[u][v] = false; // Iniciar todos os vertices como falso
            }
        }

        // Copy constructor
        Digraph(const Digraph& g)
        {
            V = g.V;
            E = g.E;
            adjMatrix = new bool*[V]; // Alocar espaço para primeira coluna
            for (int i = 0; i < V; i++)
            {
                adjMatrix[i] = new bool[V]; // Alocar espaço para cada linha
                for (int j = 0; j < V; j++)
                    adjMatrix[i][j] = false; // Iniciar todos os vertices como falso
            }
            // Copiar resultados da matriz de Adj
            for (int u = 0; u < V; u++)
            {
                for (int v = 0; v < V; v++)
                    adjMatrix[u][v] = g.adjMatrix[u][v]; // Copiar valo de cada vertice
            }            
        }

        // Destructor
        ~Digraph() {
            for(int i = 0; i < V; i++) {
                delete[] adjMatrix[i]; // Desalocar cada linha
            }
            delete[] adjMatrix; // Desalocar o restante
        }

        // Criar Grafo Transposto
        Digraph createTraverse () {
            Digraph traverse(V); // Iniciar grafo
            for (int u = 0; u < V; u++)
                for (int v = 0; v < V; v++)
                    if (adjMatrix[u][v])
                        traverse.addEdge(v, u); // Adcionar arestas
            return traverse; // Retorna grafo transposto
        }

        // Printar Grafo Transposto
        void printTraverse () {
            Digraph traverse(V); // Iniciar grafo
            for (int u = 0; u < V; u++)
                for (int v = 0; v < V; v++)
                    if (adjMatrix[u][v])
                        traverse.addEdge(v, u); // Adcionar vertices
            traverse.printMatrix(); // Retorna grafo transposto
        }



        // Criar Grafo com realacao de transitividade
        Digraph transitiveClosure () {
            Digraph closure(V); // Iniciar grafo
            for (int u = 0; u < V; u++) // Colocar arestas ligadas de u pra v
                for (int v = 0; v < V; v++)
                    if(adjMatrix[u][v])
                        closure.addEdge(u, v);
            for (int k = 0; k < V; k++)
                for (int u = 0; u < V; u++)
                    for (int v = 0; v < V; v++)
                        if (adjMatrix[u][v] || (adjMatrix[u][k] && adjMatrix[k][v]) || u == v)
                            closure.addEdge(u, v);
            return closure;
        }

        // Printar Grafo com realacao de transitividade
        void printTransitiveClosure () {
            Digraph closure(V); // Iniciar grafo
            for (int u = 0; u < V; u++) // Colocar arestas ligadas de u pra v
                for (int v = 0; v < V; v++)
                    if(adjMatrix[u][v])
                        closure.addEdge(u, v);
            for (int k = 0; k < V; k++)
                for (int u = 0; u < V; u++)
                    for (int v = 0; v < V; v++)
                        if (adjMatrix[u][v] || (adjMatrix[u][k] && adjMatrix[k][v]) || u == v)
                            closure.addEdge(u, v);
            closure.printMatrix();
        }

         // Metodo para remover loops do grafo
        bool removeLoops() {
            for (int i = 0; i < V; i++) {
                adjMatrix[i][i] = false;
            }
            return true;
        }

        // Metodo para remover ciclos do grafo
        bool removeCycles() {
            // flags
            bool* visited = new bool[V];
            bool* recStack = new bool[V];
            bool* flag = new bool;        
            bool result = false;
            // procurar ciclos no grafo
            for (int k = 0; k < V; k++) {
                // carregar variaveis de flag com bool = false
                for (int i = 0; i < V; i++) { 
                    visited[i] = false;
                    recStack[i] = false;
                }
                for (int j = 0; j < V; j++) {
                    *flag = false;
                    if (!visited[j] && removeCycleUtil(j, visited, recStack, flag))
                        result = true;
                }
            }
            return result;
        }

/*
        Ordem dos passos:
        
        1 - Find G1, the equivalente acyclic graph of g
        2 - Let G2 be formed form g1 by deleting loops
        3 - Let M1 be the incidente matrix of G2, and let M2 be the incidente matrix of GT2
        4 - Compute M3 = M1M2, and let G3 be the graph whose incidente matrix is M3
        5 - Then Gt1 is G1 - G3
        6 - Let Gt be the canonical cyclic expansion Gt1
*/


        // Metodo pra realizar uma reducao transitiva
        Digraph findTransitiveReduction() {
            Digraph g1(V); // Grafo Aciclico do Original

            // Copiar grafo original para G1
            for (int u = 0; u < V; u++) 
            {
                for (int v = 0; v < V; v++)
                    g1.adjMatrix[u][v] = adjMatrix[u][v]; // Copiar valor de cada aresta
            } 

            // Transforma G1 em aciclico
            g1.removeCycles();

            // Remover loops de g1
            g1.removeLoops();

            // O objeto G1 representa M1, ja M2 sera representada por gt2
            
            Digraph gt2 = g1.createTraverse(); // Pegar grafo com relacao de transposto de g1 
            // Computar M3 como M1 uniao com M2

            Digraph m3(V);
            for (int u = 0; u < V; u++) 
            {
                for (int v = 0; v < V; v++)
                    m3.adjMatrix[u][v] = g1.adjMatrix[u][v] && gt2.adjMatrix[u][v];
            } 

            // Computar Gt1 como de g1 com a remocao das arestas de m3
            Digraph gt1(V);

            for (int u = 0; u < V; u++) 
            {
                for (int v = 0; v < V; v++)
                    gt1.adjMatrix[u][v] = m3.adjMatrix[u][v] ? false : g1.adjMatrix[u][v];
            } 

            // Definir Gt como a expansao ciclica canonica de gt1

            Digraph gt = gt1;

            // Retornar grafo com transitividade reduzida

            return gt;      
        }

        // Adicionar Aresta
        void addEdge(int u, int v)
        {
            if (u < V && v < V) {
                adjMatrix[u][v] = true;
                this->E++;
            }   
        }

        // Remover Aresta
        void removeEdge(int u, int v)
        {
            if (u < V && v < V) {
                adjMatrix[u][v] = false;
                this->E--;
            } 
        }

        // Resgatar numero de Aresta
        int getNumberOfEdges() {
            return E;
        }

        // Resgatar numero de Vertices
        int getNumberOfNodes() {
            return V;
        }
        
        // Imprimir Matriz
        void printMatrix()
        {
            cout << "    ";
            for (int i = 0; i < V; i++) {
                cout << i << " ";
            }
            cout << endl;
            cout << "  .-";
            for (int i = 0; i < V; i++) {
                cout << "--";
            }
            cout << endl;
            for (int u = 0; u < V; u++) {
                cout << u << " | ";
                for (int v = 0; v < V; v++) {
                    cout << adjMatrix[u][v] << " ";
                }
                cout << endl;
            }
        }

        // Imprimir Matriz de uma forma mais detalhada
        void detailedPrint()
        {
            cout << "\n Imprimindo Lista de Adjacencia" << endl;
            for (int u = 0; u < V; u++) {
                cout << "\n Lista de Adjacencia de: " << u
                 << "\n ID: " << u;
                for (int v = 0; v < V; v++)
                        if (adjMatrix[u][v])
                            cout << " -> " << v;
                cout << endl;
            }
        }

        // Imprimir Numero de Arestas
        void printNumberOfEdges()
        {
            cout << "\n O Numero de arestas eh: " << getNumberOfEdges() << endl;
        }

        // Imprimir Numero de Vertices
        void printNumberOfNodes()
        {
            cout << "\n O Numero de vertices eh: " << getNumberOfNodes() << endl;
        }
};

 
// Main Code
int main()
{

    // TESTE 1

    Digraph g(5);
    
    g.addEdge(0, 0);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(4, 2);


    // TESTE 2
    /*
    Digraph g(6);
    
    g.addEdge(0, 1);
    g.addEdge(1, 0);
    g.addEdge(1, 2);
    g.addEdge(2, 3);
    g.addEdge(3, 1);
    g.addEdge(3, 4);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(4, 2);
    g.addEdge(4, 5);
    g.addEdge(5, 0);
    g.addEdge(5, 2);
    g.addEdge(5, 3);
    */

    cout << "\n";
    g.printMatrix();
    
    Digraph gt = g.findTransitiveReduction();

    cout << "\n";
    gt.printMatrix();

    return 0;
}