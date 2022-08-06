//testing rng (run using g++ on mac)

#include <iostream>
#include <string>

#include <iomanip>
#include <map>
#include <random>

int main() {

		double k = 10; //k is the energy center x-value
        double s = 0.1; //MANUAL ADJUST: height parameter - adjust based on detector accuracy??

        std::random_device rd; //generate random seed for mersenne twister
        std::mt19937 gen(rd()); //generate random numbers according to mersenne twister

        std::normal_distribution<double> dist(log(k)+pow(s,2),s); //setting (m,s) according to desmos graph, m = ln(k)+s^2 - see https://www.desmos.com/calculator/hbusvtwzmg

        std::vector<int> v(20);
        
        for( int i=0; i<200; i++){

            int num = dist(gen); //convert double to int

            if (num>=0 && num<20){
                v[num]++;
            }


        }

            for (int i=0; i<20; i++){
                std::cout << i << ": " << std::string(v[i], '*') << std::endl;
            }
            std::cout << std::endl;
            return 0;
	}


