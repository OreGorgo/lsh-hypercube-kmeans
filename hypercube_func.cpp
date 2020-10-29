#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <vector>
#include <cstring>
#include <fstream>
#include <byteswap.h>
#include <climits>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <bits/stdc++.h> 
#include <bitset>
#include <chrono> 

#include "util.h"
#include "hash.h"
#include "hypercube_func.h"

using namespace std;



vector <neighbor> knn_hypercube(hypercube_HT* ht,image query, int k,int limit,int probes)
{
	vector <neighbor> result;
	
	if(limit == 0)
		return result;
	
	neighbor temp;
	bool flag=0;
	
	std::vector<neighbor>::iterator neighbor_it;
	
	int max_dist,dist,counter=0,check=0,size,d,probes_count=0;
	unsigned int g,vertex;
	list <image>* bucket;
	
	// Create iterator pointing to first element
    std::list<image>::iterator image_it;
    	

	g = ht->hash(query); //find the query bucket
	d = ht->get_d(); //number of bits in g
	
	unsigned int bucket_no = ht->get_bucket_no();

	string gstr = bitset<32>(g).to_string(); //to binary
	vector <unsigned int> ham_vec;
	max_dist = INT_MAX;
	
	for(int i=0;i<=d;i++) //hamming distances
	{
		hamming(gstr, gstr.length()-1, i, &ham_vec,gstr.length()-d); //add all the vertices of hamming distance i to ham_vec
		
		while(!ham_vec.empty())
		{
			vertex = ham_vec.front();
			bucket = ht->get_bucket(vertex%bucket_no);
				
			size = bucket->size();
			
			image_it = bucket->begin();
			
			for(int j=0;j<size;j++) //check every element in the bucket
			{
			    dist = l1_distance(query, *image_it); //*it is bucket->at(j)
			
				if(counter < k)
				{
					neighbor_it = result.begin();
					
					//valto mesa
					for(int n=0;n<result.size();n++)
			    	{
			    		if(result.at(n).distance > dist)
			    		{
			    			temp.distance = dist;
			    			temp.p = *image_it; 
			    			
			    			
			    			result.insert(neighbor_it,temp); //save the neighbor in result as well as the distance
			    						    			
			    			counter++;			    			
							flag = 1;
			    			break;
						}
						std::advance(neighbor_it, 1);
					}
					
					if(!flag)
					{
						temp.distance = dist;
			    		temp.p = *image_it; 
			    			
			    		result.push_back(temp); //save the neighbor in result as well as the distance
			    					    					    		
			    		counter++;
					}
					
					flag=0;
					
				} //end if
				
			    else if(dist < max_dist)
			    {
			    	neighbor_it = result.begin();
			    	
			    	for(int n=0;n<k;n++)
			    	{
			    		if(result.at(n).distance > dist)
			    		{
			    			temp.distance = dist;
			    			temp.p = *image_it; 
			    			
			    			result.insert(neighbor_it,temp); //save the neighbor in result as well as the distance
					    			
			    			counter++;
			    			
			    			max_dist = result.at(k-1).distance;
			    			break;
						}
						
						std::advance(neighbor_it,1);
						
					}
										
					
				}//end else if	
				
				std::advance(image_it, 1); //get next list item
				
				
				if(++check >= limit)
					return result;
				
			
			}//end for bucket
						
			ham_vec.erase(ham_vec.begin());
			
			//probes counter
			if(++probes_count >= probes)
				return result;
		
		} //end while hamming	
		
	} //end for each hamming
	
	
    return result;
}



vector <unsigned int> range_search(hypercube_HT* ht,image query, double r, int c, int limit, int probes)
{

    vector <unsigned int> results;

	if(!limit)
		return results;
		
	int dist,counter=0,size,d,point_check=0,probes_check=0;
	unsigned int g,vertex;
	list <image>* bucket;
	
	std::list<image>::iterator image_it;

	
	double range = r*c;
	
	g = ht->hash(query); //find the query bucket
	d = ht->get_d(); //number of bits in g
	
	unsigned int bucket_no = ht->get_bucket_no();
	
	string gstr = bitset<32>(g).to_string(); //to binary
	vector <unsigned int> ham_vec;


    for(int i=0;i<=d;i++)
    {
    	hamming(gstr, gstr.length()-1, i, &ham_vec,gstr.length()-d); //add all the vertices of hamming distance i to ham_vec


    	for(int j=0;j<ham_vec.size();j++)
		{
			vertex = ham_vec[j];
			bucket = ht->get_bucket(vertex%bucket_no);
			
			size = bucket->size();


			image_it = bucket->begin();
			
			for(int k=0;k<size;k++) //check every element in the bucket
	        {
	            dist = l1_distance(query, *image_it);
				
	            if(dist <= range)
	            {
					results.push_back(image_it->id);
				}	
				
				std::advance(image_it,1);
				
				if(++point_check >= limit)
					return results;
				
	        }
	        
			if(++probes_check >= probes)
			    return results;
		}

    	ham_vec.clear();
        
        //cout <<endl;

    }


    return results;
}

