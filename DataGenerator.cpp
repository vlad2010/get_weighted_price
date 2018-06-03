#define ORDERS 100000
#define ID_BEGINS 100 
#define TIMESTAMP_START 1000 

#define NEXT_TIMESTAMP 1000
#define ERASE_ORDER_STEP 100000 

#define MINPRICE 1.0
#define MAXPRICE 500.0

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <limits>
#include <math.h>  
#include <stdlib.h>     /* atoi */

#include <iostream>
#include <cmath>
#include <limits>

using namespace std;


enum OperationType {
	INSERT = (int)'I',
	ERASE = (int)'E',
};

struct Order {
	unsigned int timestamp{ 0 };
	char operationType;
	unsigned int id{ 0 };
	double price{ 0.0 };

	friend std::ostream &operator<<(std::ostream &os, Order const &order);
};

std::ostream &operator<<(std::ostream &os, Order const &order) {

	if (order.operationType == INSERT) {


		// string suffix = (order.price - (unsigned int)(order.price) > 0) ?"":".0";

		std::cout.setf(std::ios::fixed, std::ios::floatfield);

//		string strPrice ;

		// std::ostringstream strs;
		// strs << dbl << suffix;
		// std::string strPrice = strs.str();

		// if(order.id==328131) {
			
		// 	cout << "Delta: " << (order.price - (unsigned int)(order.price) > 0);
		// 	cout << "Price: " << order.price  << " int order price:"  << (unsigned int)(order.price) ;


		//     double f3;
		//     double f2 = std::modf(order.price, &f3);
		//     std::cout << "order.price : modf() makes " << f3 << " + " << f2 << '\n';

		// 	cout << "Suffix:" << suffix << ": Price:" << order.price << endl;
		// 	exit(0);
		// }

		return os << order.timestamp << " " << order.operationType << " " << order.id << " " << order.price << endl;
	}
	else {
		return os << order.timestamp << " " << order.operationType << " " << order.id << endl;
	}
}
//
//struct OrderStr {
//	unsigned int timestamp{ 0 };
//	unsigned int end_timestamp{ 0 };
//
//	unsigned int id{ 0 };
//	double price{ 0.0 };
//};
//
//std::ostream &operator<<(std::ostream &os, OrderStr const &m) {
//	return os << "ts:" << m.timestamp << " ts_end:" << m.end_timestamp << " id:" << m.id << " price:" << m.price << endl;
//}

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

class OrderBook {

public:
	void insert(Order &order) {
		orders[order.id] = order;
		prices.insert(order.price);
	};

	void erase(Order &order) {
		Order &insertOrder = orders[order.id];
		//cout << "Price to delete: " << insertOrder.price << endl;

		//find and delete only one price. we can't use prices.erase(insertOrder.price); here
		auto priceToDeletePtr = prices.find(insertOrder.price);
		if (priceToDeletePtr != prices.end()) {
			prices.erase(priceToDeletePtr);
		}

		orders.erase(order.id); //erase Order by key

								//cout << "Order deleted: " << order << endl;
								//cout << "Highest price after delete: " << getHighestPrice() << endl;
	};

	double getHighestPrice() const {
		if (!prices.size()) {
			return std::numeric_limits<double>::quiet_NaN();
		}

		// maximum value is in the end of multimap  	
		return *(std::prev(prices.end()));
	};

	friend std::ostream &operator<<(std::ostream &os, const OrderBook &book);

private:
	map<unsigned int, Order> orders;  //map with incoming orders, key is id  
	multiset<double> prices;          //multimap is sorted container, we can easily get max price 
};

std::ostream &operator<<(std::ostream &os, const OrderBook &book) {
	return os << "Number of orders: " << book.orders.size() << " Max price: " << book.getHighestPrice() << endl;
}



int main(int argc, char* argv[])
{

	int ordersToGen = ORDERS;

	if(argc>1) {
		ordersToGen = atoi (argv[1]);
	}

	unsigned int timestamp = 1000;

	//vector<OrderStr> orders;

	map<unsigned int, Order> ordersMp;
	
	unsigned int next_id = ID_BEGINS;

	for (int i = 0; i < ordersToGen; i++) {
		
		
		//OrderStr o;

	/*	o.timestamp = timestamp + rand() % NEXT_TIMESTAMP;
		o.end_timestamp = o.timestamp + rand() % ERASE_ORDER_STEP;
		o.id = next_id++;
		o.price = fRand(5.0, 50.0);*/



		//cout << i << ") " << o << endl;
		//orders.push_back(o);

		Order insertOrder;
		insertOrder.timestamp = timestamp + rand() % NEXT_TIMESTAMP;;
		insertOrder.operationType = INSERT;
		insertOrder.id = next_id++;
		insertOrder.price = fRand(MINPRICE, MAXPRICE);
		
		Order eraseOrder;
		eraseOrder.timestamp = insertOrder.timestamp + rand() % ERASE_ORDER_STEP;
		eraseOrder.operationType = ERASE;
		eraseOrder.id = insertOrder.id;
		eraseOrder.price = 0.0;

		ordersMp[insertOrder.timestamp] = insertOrder;
		ordersMp[eraseOrder.timestamp] = eraseOrder;

		timestamp = insertOrder.timestamp + 1;
	}


	//now iterate map to see


	//for (pair<unsigned int, Order> &o : ordersMp) {
	std::ofstream file("output.txt");

	
	//calculate price on the fly 
	unsigned int previousTime = 0;
	unsigned timeTakeIntoAccount = 0;
	
	double previousHighestPrice = 0.0;
	double weightedPrice = 0.0;

	OrderBook bk;

	  // file << std::fixed;
	  // file << std::setprecision(4);

    for (auto &o : ordersMp) {

		//cout << "Order: " << o.second;
		file << o.second;

		Order &order = o.second;

		(order.operationType == INSERT) ? bk.insert(order) : bk.erase(order);

		double currentHighestPrice = bk.getHighestPrice();


		//initial or pause 
		if (previousTime == 0) {
			previousTime = order.timestamp;
			previousHighestPrice = currentHighestPrice;
			continue;
		}

		
		if (currentHighestPrice != previousHighestPrice) {

			unsigned int deltaTime = (order.timestamp - previousTime);
			weightedPrice = weightedPrice + previousHighestPrice * deltaTime;
			timeTakeIntoAccount = timeTakeIntoAccount + deltaTime;


			if (isnan(currentHighestPrice)) {
				previousTime = 0;
				previousHighestPrice = 0.0;
			}
			else {
				previousTime = order.timestamp;
				previousHighestPrice = currentHighestPrice;
			}

		}



		
	}


	double finalPrice = weightedPrice / timeTakeIntoAccount;

	cout << "Final weighted high price: " << finalPrice << endl;
	
	file  << "Final weighted high price: " << finalPrice << endl;

	return 0;
}

