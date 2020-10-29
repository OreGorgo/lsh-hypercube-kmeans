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
#include "kmeans_func.h"

//$./cluster –i <input file> –c <configuration file> -o <output file> -complete
//<optional> -m <method: Classic OR LSH or Hypercube>

using namespace std;
using namespace std::chrono; 





int main(int argc,char **argv)
{
    string input_file,config_file,output_file,method;

    bool complete;

    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"-i") == 0)
        {
            i++;
            input_file=argv[i];
        }


        if(strcmp(argv[i],"-c") == 0)
        {
            i++;
            config_file=argv[i];
        }

        if(strcmp(argv[i],"-o") == 0)
        {
            i++;
            output_file=argv[i];
        }


        if(strcmp(argv[i],"-m") == 0)
        {
            i++;
            method=argv[i];
        }


        if(strcmp(argv[i],"-complete") == 0)
        {
            i++;
            complete=true;
        }

    }



    if(input_file.empty())
    {
        cout<<"Give input file!"<<endl;
        cin >> input_file;
    }


    if(config_file.empty())
    {
        cout<<"Give config file!"<<endl;
        cin >> config_file;
    }

    if(output_file.empty())
    {
        cout<<"Give output file!"<<endl;
        cin >> output_file;
    }

	
	
    int images,rows,cols;

    ifstream fp(input_file.c_str(),  ios::in | ios::binary );
    if (!fp.is_open())
    {
        cout << "Unable to open file";
        return 1;
    }

    srand(time(nullptr));


    //get metadata
    fp.seekg(4,fp.beg);
    fp.read ((char*)&images,4);

    images = bswap_32(images);

    //fp.seekg(8,fp.beg);
    fp.read((char*)&rows,4);

    rows = bswap_32(rows);

    //fp.seekg(12,fp.beg);
    fp.read((char*)&cols,4);

    cols = bswap_32(cols);

    //Data starts here
    //fp.seekg(16,fp.beg);


    cout << images <<endl;
    cout << rows <<endl;
    cout << cols <<endl;


	images = 1000;

    image* arr = new image[images];
    unsigned char temp;
    

    int size = rows*cols;

    for(int j=0;j<images;j++)
    {
    	arr[j].id = j;
        arr[j].cluster = -1;
        arr[j].old_cluster = -1;
        arr[j].g = 0;
        
		for(int i=0; i<size; i++)
        {
            fp >> temp;
            
            arr[j].pixels.push_back(temp);

            //cout << (int) arr[j].pixels.at(i) <<" ";
        }

    }
    
    
    int K = 4; //KMEANS
    
	int L = 3; //number of hashtables
    int lsh_k = 4;//number of h for LSH
    
    int cube_M = 10;//hypercube M
    int cube_k = 3;//hypercube k
    int probes = 2;//number of probes for hypercube
    
    
    char* input;
    
    //reading of config file
   	ifstream conf(config_file.c_str());
	if (conf.is_open()) {
	  	string line;
	    while (std::getline(conf, line)) {
	        
	        if(strncmp("number_of_clusters:",line.c_str(),strlen("number_of_clusters:")) == 0)
	        {
	        	input = (char*)line.c_str();
	        	input += strlen("number_of_clusters:");
	        	while(*input == ' ')
	        		input++;
	        	
	        	K = atoi(input);
			}
			else if(strncmp("number_of_vector_hash_tables:",line.c_str(),strlen("number_of_vector_hash_tables:")) == 0)
	        {
	        	input = (char*) line.c_str() + strlen("number_of_vector_hash_tables:");
	        	while(*input == ' ')
	        		input++;
	        	
	        	L = atoi(input);
			}
			else if(strncmp("number_of_vector_hash_functions:",line.c_str(),strlen("number_of_vector_hash_functions:")) == 0)
	        {
	        	input = (char*) line.c_str() + strlen("number_of_vector_hash_functions:");
	        	while(*input == ' ')
	        		input++;
	        	
	        	lsh_k = atoi(input);
			}
			else if(strncmp("max_number_M_hypercube:",line.c_str(),strlen("max_number_M_hypercube:")) == 0)
	        {
	        	input = (char*) line.c_str() + strlen("max_number_M_hypercube:");
	        	while(*input == ' ')
	        		input++;
	        	
	        	cube_M = atoi(input);
			}
			else if(strncmp("number_of_hypercube_dimensions:",line.c_str(),strlen("number_of_hypercube_dimensions:")) == 0)
	        {
	        	input = (char*) line.c_str() + strlen("number_of_hypercube_dimensions:");
	        	while(*input == ' ')
	        		input++;
	        	
	        	cube_k = atoi(input);
			}
			else if(strncmp("number_of_probes:",line.c_str(),strlen("number_of_probes:")) == 0)
	        {
	        	input = (char*) line.c_str() + strlen("number_of_probes:");
	        	while(*input == ' ')
	        		input++;
	        	
	        	probes = atoi(input);
			}
			
	        
	    }
	    conf.close();
	} 
    
	//int w = average_NN(arr, 500);
    int w = 10000;
    
    int threshold = images/500;


    ofstream output_fp;
    output_fp.open (output_file);

    vector<cluster> clusters;
    vector <double> sil;


    auto start = high_resolution_clock::now();

	if(!strcmp(method.c_str(),"LSH"))
        clusters = kmeans_lsh(arr, size, images, K, L, lsh_k, w, threshold);

	else if(!strcmp(method.c_str(),"Hypercube"))
        clusters = kmeans_hypercube(arr, size, images, K, cube_k, cube_M, probes, w, lsh_k, threshold);

	else
        clusters = kmeans_lloyds(arr, size, images, K, threshold);


    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

	for(int i=0;i<clusters.size();i++)
    {
	    output_fp<<"CLUSTER-"<<i+1<<" {size: "<<clusters[i].points.size()<<" , "<<"centroid [ ";
	    for(int j=0;j<size;j++)
	    {
            output_fp << clusters[i].centroid.pixels[j];
            if (j < size - 1)
                output_fp << " , ";
        }

	    output_fp<<" ] }"<<endl;
    }


    output_fp <<"clustering_time: "<< (double)duration.count()/1000000.0 << endl;


    sil = silhouette(arr,images,clusters);

    output_fp<<"Silhouette: [ ";
    for(int i=0;i<sil.size();i++)
    {
        output_fp<<sil[i];
        if(i<sil.size()-1)
            output_fp<<" , ";
    }
    output_fp<<" ]";



    complete=true;
    if(complete)
    {
        for(int i=0;i<clusters.size();i++)
        {
            output_fp << "CLUSTER-" << i+1 << " { " <<clusters[i].centroid.id<< " , ";
            for(int j=0;j<clusters[i].points.size();j++)
            {
                output_fp << clusters[i].points[j].id;
                if (j < clusters[i].points.size()-1 )
                    output_fp<<" , ";
            }
            output_fp<<" }"<<endl;
        }
    }
    //vector<cluster> clusters = kmeans_lsh(arr, size, images, K, L, lsh_k, w, threshold);
	//vector<cluster> clusters = kmeans_hypercube(arr, size, images, K, cube_k, cube_M, probes, w, lsh_k, threshold);
	
	


	delete[] arr;
    fp.close();
    
    
    return 0;
}



//train-images.idx3-ubyte

