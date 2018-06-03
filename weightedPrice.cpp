#include <iostream>
#include <memory>
#include <stack>
#include <map>
#include <string>

#include <iostream>
#include <fstream>
#include <limits>
#include <regex>

using namespace std;

void printUsage();
inline bool exists_test0 (const char* path);

enum OperationType{
    INSERT = (int)'I',
    ERASE  = (int)'E',
};

struct Order {
	unsigned int timestamp{0};
	char operationType;	
	unsigned int id{0};
	double price{0.0};
};

std::ostream &operator<<(std::ostream &os, Order const &m) { 
    return os << "Timestamp:" << m.timestamp << " Type:" << m.operationType << " id:" << m.id << " Price:" << m.price << endl;
}

// struct InsertOrder : public Order {
// 	double price;
// };

struct OrderRecord {
    unsigned int timestampStart;
    unsigned int timestampFinish;
	unsigned int id;
	double price;
};

// const std::string regexOrder = "(^\\d{1,10})[[:space:]]([I|E])[[:space:]](\\d{1,10})[[:space:]]?(\\d{1,10}.\\d{1,10})?";

class OrderBook {
public:
	OrderBook(){ cout << "OrderBook ctor" << endl; };
	~OrderBook(){ cout << "OrderBook dtor" << endl; };


    void insert(Order &o) {

    	orders[o.id] = o;

    	if(highestPrice>o.price) {

    		pair<int,double> p{o.id, o.price};

    		highestPriceOrdersIdStack.push(p);
    		highestPrice=o.price;
    	}
    };

    void erase(Order &o) {

    	//check if we delete order with highest price 

    	orders.erase[o.id];
    };
    
    double getAverageHighestPrice(){ 

    	if(!orders.size()) {
    		return std::numeric_limits<double>::quiet_NaN();
        }

        return highestPrice;
    };

private:	
	map<int, Order> orders;
	
	stack<pair<int,double> > highestPriceOrdersIdStack;  // stack of orders id and price pairs
	double highestPrice{0};
};


class Application {
public:
	double run(string path) {


			//open file 
            std::ifstream infile(path);

            // unsigned int ts;
            // char id;
            // unsigned int type;
            // double price; 
            // while (infile >> ts >> id >> type >> price) {

            // 	cout << "ts: " << ts << " id:" << id << " type:" << type << " price:" << price << endl;
            // }

            OrderBook bk;

			std::string line;
			while (std::getline(infile, line))
			{
				cout << "next line: " << line << endl;	
				Order o = getOrderFromLine(line);

				bk.insert(o);
			}


            cout << "end parsing " << endl;	
				     

				     double pr = bk.getAverageHighestPrice();
				     cout << "Average highest price: " << pr << endl;


		return 0.0;
	}

	Order getOrderFromLine(string &line) {

	   Order o;
	   std::smatch match;
       const std::regex re("(^\\d{1,10})[[:space:]]([I|E])[[:space:]](\\d{1,10})[[:space:]]?(\\d{1,10}.\\d{1,10})?");

       bool res = std::regex_search(line, match, re);

       cout << "res: " << res << " Matched: " << match.size() << endl;
       if(res) {
	       //   for (auto &a: match) {
		   //   cout << "match: " << a << endl;
		   // }

		   cout << "match[1]: " << match[1] << endl;
		   cout << "match[2]: " << match[2] << endl;
		   cout << "match[3]: " << match[3] << endl;
		   cout << "match[4]: " << match[4] << endl;

		   o.timestamp = stoi(match[1]);
		   o.operationType = ((string)match[2]).at(0);
		   o.id = stoi(match[3]);
		   if(o.operationType == INSERT)
		   		o.price = stod(match[4]);
       }

       cout << "Current order: " << o << endl;

	   return o;
	}



};

int main(int argc, char* argv[]) {

	 int returnCode=0;

	 if(argc>1) {
	     
	     clog << "weighted price demo" << endl;

	     clog << "File name: " << argv[1] << endl;

	     //check file exist	
	     if(exists_test0(argv[1]) ) {
				     

	     			 Application app;	
	     			 app.run(string(argv[1]));

	     } else {
			cerr << "Error: file not exists" << endl;
			returnCode-1;
	     }
     } 
     else {
     	printUsage();
     	returnCode=-1;
     }

     return returnCode;
}


inline bool exists_test0 (const char* path) {
    ifstream f(path);
    return f.good();
}

void printUsage() {
	cerr << "Usage: weightedPrice <OrderDataFile>" << endl;
}


