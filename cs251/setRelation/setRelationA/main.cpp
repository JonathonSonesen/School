/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Evaluate relations between domain (set A) and codomain(set B) 
 *                  this porgram could use optimization, and will be improved later
 *
 *        Version:  1.0
 *        Created:  04/24/2015 12:38:31 AM
 *       Revision:  none
 *       Compiler:  g++ -std=c++11 main.cpp
 *
 *         Author:  Jonathon Sonesen
 *  Collaborators:  Jeff Patterson
 *   Organization:  PCC CS251 Dicrete StructuresII
 *
 * =====================================================================================
 */
#include <iostream>                         
#include <algorithm>                                            
#include <string>
#include <set>
#include <tuple>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

            
typedef string elementtype;                                     //Datatpe for domain and codomain elements
typedef set<elementtype> settype;                               //Sets of strings 
typedef set<tuple<elementtype, elementtype>> crosstype;         //Cross product is a tuple
typedef set<crosstype> powertype;                               //Power set is a set of sets

//Functions to generate sets
powertype * powerset(crosstype *);                             //power set generator 
crosstype * crossproduct(settype *, settype *);                //cross product generator
settype   * loadSet(string);                                     //load domain or codomain from user

//Functions to check properties of power set elements
//elements are ordered pairs
bool isFunction(crosstype relation, settype * domain);
bool isInjective(crosstype relation, settype *domain, settype *codomain);
bool isSurjective(crosstype relation, settype * domain, settype *codomain);
bool isBijective(crosstype relation, settype * domain, settype *codomain);
bool isTransitive(crosstype relation, settype * domain);
bool isReflexive(crosstype relation, settype * domain);
bool isAntiSym(crosstype relation, settype * domain);
bool isSym(crosstype relation, settype * domain);


int main ()
{
    settype * domain   = loadSet("Enter set A: ");                       //load domain
    settype * codomain = loadSet("Enter set B: ");                     //load codomain
    crosstype * cross  = crossproduct(domain, codomain);          //generate cross product
    powertype * power  = powerset(cross);                         //generate the power set


    //Print coss product roster. The cross product is a set of ordered pairs. They
    //are represented with the <tuple> container. get is used to access the 0 or
    //1 element of the ordered pair. Then conatiner iterators can be used to run through
    cout << "Cross Product: {";
    for(auto ptr = cross->begin(); ptr != cross->end(); ptr++)
    {
        cout << " (" << std::get<0>(*ptr) << "," << std::get<1>(*ptr) << ") ";
    }
    cout << "}" << endl;

    //Print out the power set roster. The power set is a set of sets. In other words
    //each element of the powerset is a set of tuple. So once you access a particular
    //element of that power set you need a second loop to acess the the tuples of the element
    //
    //we also use filters to identify properties of the elements, such as surjectiveness
    //injectiveness and bijectiveness
    //This prints the number of functions and tests the power set
    
    cout << endl;
 
    //Bijective
    cout << " bijective functions from A to B: " <<  endl;
    auto row = 0;
    for (auto ptrA = power->begin(); ptrA != power->end(); ptrA++)
    {    
        if(isBijective(*ptrA, domain, codomain))
        {
            cout << setw(3) << setfill (' ') << ++row << ". " << "{";
            
            //Loop fo inner tuples
            for(auto ptr = ptrA->begin(); ptr != ptrA->end();ptr++)
            {
                cout << " (" << std::get<0>(*ptr) << "," << std::get<1>(*ptr) << ") ";
            }
            cout << "}" << endl;
        }
    }
    cout << endl;
   
    //Print equivalence relations
    cout  << " Injective Functions: " <<  endl;
    row = 0;
    for (auto ptrA = power->begin(); ptrA != power->end(); ptrA++)
    {    
        if(isInjective(*ptrA, domain, codomain))
        {
            cout << setw(3) << setfill (' ') << ++row << ". " << "{";   
            
            //Loop fo inner tuples
            for(auto ptr = ptrA->begin(); ptr != ptrA->end();ptr++)
            {
                cout << " (" << std::get<0>(*ptr) << "," << std::get<1>(*ptr) << ") ";
            }
            cout << "}" << endl;
        }
    }
    cout << endl;
   
    //Equivalence Relations:
    cout << " Equivalence relations: " <<  endl;
    row = 0;
    for (auto ptrA = power->begin(); ptrA != power->end(); ptrA++)
    {    
        if(isTransitive(*ptrA, domain)&&isReflexive(*ptrA, domain) && isSym(*ptrA,domain))
        {
            cout << setw(3) << setfill (' ') << ++row << ". " << "{";
            
            //Loop fo inner tuples
            for(auto ptr = ptrA->begin(); ptr != ptrA->end();ptr++)
            {
                cout << " (" << std::get<0>(*ptr) << "," << std::get<1>(*ptr) << ") ";
            }
            cout << "}" << endl;
        }
    }
    cout << endl;
    
    //Partial Order Relations:
    cout << " Partial Order relations: " <<  endl;
    row = 0;
    for (auto ptrA = power->begin(); ptrA != power->end(); ptrA++)
    {    
        if(isTransitive(*ptrA, domain)&&isReflexive(*ptrA, domain) && isAntiSym(*ptrA,domain))
        {
            cout << setw(3) << setfill (' ') << ++row << ". " << "{";
            
            //Loop fo inner tuples
            for(auto ptr = ptrA->begin(); ptr != ptrA->end();ptr++)
            {
                cout << " (" << std::get<0>(*ptr) << "," << std::get<1>(*ptr) << ") ";
            }
            cout << "}" << endl;
        }
    }
    cout << endl;
    //Leak stopper five thousand
    delete domain;
    delete codomain;
    delete cross;
    delete power;
    return 0;
}




/* Filter: Surjective Function
 * Parm: crosstype powerset, settype domain,settype codomain
 * Return: true if function is surjective
 */
bool isSurjective( crosstype  relation, settype * domain, settype * codomain)
{
    //Prelims
    if(!isFunction(relation, domain))     return false;
    if(codomain->size() > domain->size()) return false;
    
    //Check that appears all elements of codomain are in relation set
    set<elementtype> relem;
    for(auto ptr = relation.begin(); ptr != relation.end(); ptr++)
    {  
        //Get codomain element
        auto buffer = std::get<1>(*ptr);
        relem.insert(buffer);
    }
    //If each codomain element is found it is surjective
    if (relem.size() == codomain->size()) return true;
    return false;
}

/* Filter: Function
 * Parm: crosstype powerset, settype domain
 * Return: true is relation is a function false otherwise
 * Check domain consumption and unique image 
 * */
bool isFunction( crosstype relation, settype * domain)
{
   set<elementtype> relem;           //Set of elements from the tuples

   for (auto ptr = relation.begin();ptr != relation.end(); ptr++)
   {
        auto buffer = std::get<0>(*ptr);        //Get first element of ordered pair
        relem.insert(buffer);                   //Add to tuples
   }
   if (relem.size() != domain->size())    return false;
   if (relation.size() != domain->size()) return false;
   return true;
}

/* Filter: Injective Functions
 * Parm: powerset, domain codomain
 * Return: true is isInjective
 */
bool isInjective( crosstype  relation, settype * domain, settype * codomain)
{
    if(isFunction(relation, domain) == false) return false;
    if(codomain->size() < domain->size())     return false;


    //Make sure codomain element appears once
    set<elementtype> relem;
    for(auto ptr = relation.begin(); ptr != relation.end(); ptr++)
    {
        //Get codomain cotributed to ordered pair
        auto buffer = std::get<1>(*ptr);
        if (relem.find(buffer) != relem.end()) return false;
        relem.insert(buffer);
    }
    return true;
}

/*Filter: Bijective function
 * Parm: powerset, domain, codomain
 * Return: true function is both injective and surjective
 */
bool isBijective( crosstype relation, settype *domain, settype * codomain)
{
    if( !isFunction(relation, domain)) return false;
    if(!isSurjective(relation, domain, codomain)) return false;
    if(!isInjective(relation, domain, codomain)) return false;

    
    return true;
}

/* Filter: Transitivity
 * Parm: Power set and domain
 * Return: True is transitivity holds
 */
bool isTransitive (crosstype relation, settype * domain)
{
    //domain check for relation
    if(relation.size() < domain->size()) return false;
    for (auto ptrx = relation.begin(); ptrx != relation.end();ptrx++)
    {
        auto bufferx = std::get<0>(*ptrx);
        for (auto ptry = relation.begin();ptry != relation.end(); ptry++)
        {
            auto buffery = std::get<1>(*ptry);
            auto xy = std::make_tuple(bufferx,buffery);
            if(relation.find(xy) != relation.end())
            {
                for(auto ptrz = relation.begin(); ptrz != relation.end(); ptrz++)
                {
                    auto bufferz = std::get<1>(*ptrz);
                    auto yz = std::make_tuple(buffery, bufferz);
                    auto xz = std::make_tuple(bufferx, bufferz);
                    if (relation.find(yz) != relation.end() && relation.find(xz) == relation.end())
                        return false;
                }
            }
        }
    }
    return true;
}
/* Filter: Symmetry
 * Parm: Power set, domain
 * Return:True if symmetry holds
 */
bool isSym(crosstype relation, settype * domain)
{
    if(relation.size() < domain->size()) return false;
    for( auto ptr = relation.begin(); ptr != relation.end(); ptr++)
    {
        auto bufferx = std::get<0>(*ptr);
        auto buffery = std::get<1>(*ptr);
        auto yx = std::make_tuple(buffery, bufferx);
        if (relation.find(yx) == relation.end()) return false;
    }
    return true;
}
/* Filter: Reflexivity
 * Parm: powerset domain
 * Return: true is relation is reflexive
 */
bool isReflexive(crosstype relation, settype * domain)
{
    //domain size for relation
    if( relation.size() < domain->size()) return false;
    for(auto ptr = domain->begin(); ptr != domain->end(); ptr++)
    {
        auto bufferx = *ptr;
        auto xx = std::make_tuple(bufferx,bufferx);
        if(relation.find(xx) == relation.end()) return false;
    }
    return true;
}
/* Filter: Anti Symmetry
 * Parm: powerset, domain
 * ReturnL true is anti sym holds
 */
bool isAntiSym(crosstype relation, settype *domain)
{
    if(relation.size() < domain->size()) return false;
    for(auto ptr = relation.begin(); ptr != relation.end(); ptr++)
    {
        auto bufferx = std::get<0>(*ptr);
        auto buffery = std::get<1>(*ptr);
        auto yx = std::make_tuple(buffery,bufferx);
        if(relation.find(yx) != relation.end() && bufferx != buffery)
            return false;
    }
    return true;
}

/*Create power set
 * Parm: Address of cross product set
 * Return: Address of power set of cross product
 * 
 */
powertype * powerset(crosstype * cross)
{
    typedef crosstype::iterator sPtr;
    typedef std::vector<sPtr>vec;
    typedef vec::iterator vPtr;
    struct local
    {
        static tuple<elementtype, elementtype> dereference(sPtr v)
        {
            return *v; 
        }
    };
    powertype * result = new powertype;                                 //the resulting powersey
    vec elements;                                                       //element stack

    do 
    {
        crosstype tmp;

        //copy initial elemens and insert them into the power set element
        std::transform(elements.begin(), elements.end(), std::inserter(tmp, tmp.end()), local::dereference);
        result->insert(tmp);

        //Check the stack for more elements. If more exist, pump them to the stack
        if(!elements.empty() && ++elements.back() == cross->end())
        {
            elements.pop_back();
        } else {
            sPtr iter;
            if(elements.empty() == true)
            {
                iter = cross->begin();
            } else {
                iter = elements.back();
                ++iter;
            }
            for(; iter != cross->end(); ++iter)
            {
                elements.push_back(iter);
            }
        }
    } while (!elements.empty());
    return result;
}

/* Load set
 * Parm: Name of set
 * Return: Address of set
 */
settype * loadSet(string parm)
{
    settype * result = new settype;                             //Result
    elementtype input;                                          //user input buffer

    cout << "Enter elements for " << parm << " (space seperated): ";    //prompt
    while (cin.peek() != '\n') 
    {
        cin >> input;                                           //Read element
        result->insert(input);                                  //Add to set
    }
    cin.ignore(8192, '\n');                                     //eat end line
    return result;
}

/* Generate cross product
 * Parm: domain, co-domain sets
 * Return: address of cross product
 */
crosstype * crossproduct(settype * domain, settype * codomain)
{
    crosstype * result = new crosstype;
    elementtype first, second;

    for(settype::iterator ptrA = domain->begin(); ptrA != domain->end(); ptrA++)
    {
        for (settype::iterator ptrB = codomain->begin(); ptrB != codomain->end(); ptrB++)
        {
            auto element = make_tuple(*ptrA, *ptrB);
            result->insert(element);
        }
    }
    return result;
}
