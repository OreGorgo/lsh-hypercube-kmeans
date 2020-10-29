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
using namespace std::chrono; 


//k=14, M=10, probes=2, N=1, R=10000.

int main(int argc,char **argv)
{
    string data_path,query_path,output_file;
    int k=7,limit=10,N=1,probes=2;
	double R=10000.0;

    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"-d") == 0)
        {
            i++;
            data_path=argv[i];
        }

        if(strcmp(argv[i],"-q") == 0)
        {
            i++;
            query_path=argv[i];
        }

        if(strcmp(argv[i],"-o") == 0)
        {
            i++;
            output_file=argv[i];
        }


        if(strcmp(argv[i],"-k") == 0)
        {
            i++;
            k=atoi(argv[i]);
        }

        if(strcmp(argv[i],"-M") == 0)
        {
            i++;
            limit=atoi(argv[i]);
        }
        
        if(strcmp(argv[i],"-probes") == 0)
        {
            i++;
            probes=atoi(argv[i]);
        }

        if(strcmp(argv[i],"-N") == 0)
        {
            i++;
            N=atoi(argv[i]);
        }


        if(strcmp(argv[i],"-R") == 0)
        {
            i++;
            R=atof(argv[i]);
        }


    }



    if(data_path.empty())
    {
        cout<<"Give dataset path!"<<endl;
        cin >> data_path;
    }


    if(query_path.empty())
    {
        cout<<"Give query path!"<<endl;
        cin >> query_path;
    }

    if(output_file.empty())
    {
        cout<<"Give output path!"<<endl;
        cin >> output_file;
    }




    int images,rows,cols;

    ifstream fp(data_path.c_str(),  ios::in | ios::binary );
    if (!fp.is_open())
    {
        cout << "Unable to open file";
        return 1;
    }


    //get metadata
    fp.seekg(4,fp.beg);
    fp.read ((char*)&images,4);

    images = bswap_32(images);

    fp.seekg(8,fp.beg);
    fp.read((char*)&rows,4);

    rows = bswap_32(rows);

    fp.seekg(12,fp.beg);
    fp.read((char*)&cols,4);

    cols = bswap_32(cols);

    //Data starts here
    fp.seekg(16,fp.beg);


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
        }

        //cout <<endl;
    }


    //int w = average_NN(arr, 500);
    int w = 40000;


	int d;

    unsigned int M = pow(2,32/k); //modulo number
    unsigned int buckets = images/16;
    vector <double>* S_arr;
    
    //hash -> vector apo listes -> list:int
    
    
    
        
    //random engine for uniform distribution
    default_random_engine generator;
    uniform_real_distribution<double> distribution(0.0, w);

	
	hypercube_HT* hyp;
	
	
	S_arr = compute_s(w,k,size,distribution,&generator); //compute the d*k different s for the hashtable
	hyp = new hypercube_HT(k,size,k,w,M,S_arr);
    
    srand(time(NULL));
    for(int i=0;i<images;i++)
    {
    	hyp->insert_image(arr[i]);	
    }

    ifstream query_fp(query_path.c_str(),  ios::in | ios::binary );
    if (!query_fp.is_open())
    {
        cout << "Unable to open file";
        return 1;
    }

   	vector <image> query_arr;
	image temp_image;
	int query_images=0;
	int aa;
	
	//get metadata
    query_fp.seekg(4,query_fp.beg);
    query_fp.read ((char*)&aa,4);
    query_images = bswap_32(aa);
    query_fp.read((char*)&aa,4);
    query_fp.read((char*)&aa,4);
    for(int j=0;j<query_images;j++)
    {
	query_arr.push_back(temp_image);
        query_arr[j].id = j;
        query_arr[j].cluster = -1;
        query_arr[j].old_cluster = -1;
        query_arr[j].g = 0;


        for(int i=0; i<size; i++)
        {
            query_fp >> temp;
            query_arr[j].pixels.push_back(temp);
        }
                
    }


    vector <neighbor> hyp_result;
    vector <unsigned int> brute_result;
    vector <unsigned int> range_result;



    double thyp=0,ttrue=0;

    ofstream output_fp;
    output_fp.open (output_file);
    
    //query_images=10;


    for(int i=0;i<query_images;i++)
    {
        auto start = high_resolution_clock::now();

        hyp_result = knn_hypercube(hyp, query_arr[i], N, limit,probes);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        thyp = (double)duration.count()/1000000.0;

        start = high_resolution_clock::now();

        brute_result = brute_force_NN(arr,query_arr[i],images);

        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        ttrue = (double)duration.count()/1000000.0;


        output_fp<<"Query: "<<i+1<<endl<<endl;

        for(int j=0;j<k;j++)
        {
            output_fp<<"neighbor-"<<j+1<<": "<< hyp_result[j].p.id+1<<endl;
            output_fp<<"distanceHypercube: "<<hyp_result[j].distance<<endl;
            output_fp<<"distanceTrue: "<<brute_result[j]<<endl<<endl;
        }

        output_fp <<"tHypercube: "<<thyp  << endl;
        output_fp <<"tTrue: "<<ttrue  << endl<<endl;

        output_fp<< "R-Near neighbors:"<<endl;

        range_result=range_search(hyp,query_arr[i], R, 1, limit,probes);
        for(int r=0;r<range_result.size();r++)
            output_fp<<range_result[r]+1<<endl;

        output_fp<<endl;
    }
    //  ../t10k-images.idx3-ubyte

    output_fp.close();




    fp.close();
    
	delete[] arr;
	delete hyp;
    return 0;
}

//train-images.idx3-ubyte

