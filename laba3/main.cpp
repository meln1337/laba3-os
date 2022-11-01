#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <chrono>
#include <omp.h>
#include <string>
//#include <Windows.h>

#define MAX_THREADS 1000

using namespace std;
typedef long long int ll;

//typedef struct MyData {
//	vector<vector<ll>> a, b, c;
//	vector<pair<int, int>> indexes;
//} MYDATA, *PMYDATA;


void generate_file(ll n, ll m, ll d, ll e)
{
	ofstream file(to_string(n) + "x" + to_string(m) + "_" + to_string(d) + "x" + to_string(e) + ".txt");
	file << n << " " << m << endl;
	ll k = 0;
	for (ll i = 0; i < n; i++)
	{
		for (ll j = 0; j < m; j++)
		{
			k++;
			if (j == m - 1)
			{
				file << k;
			}
			else
			{
				file << k << " ";
			}
		}
		file << endl;
	}
	file << d << " " << e;
	k = 0;
	for (ll i = 0; i < d; i++)
	{
		for (ll j = 0; j < e; j++)
		{
			k++;
			if (j == e - 1)
			{
				file << k;
			}
			else
			{
				file << k << " ";
			}
		}
		file << endl;
	}
	file.close();
}

vector<vector<ll>> matrix_multiplication(vector<vector<ll>> a, vector<vector<ll>> b)
{
// a (n, m), b (d, e) => res (n, e)
	auto start = chrono::high_resolution_clock::now();
	ll n = a.size();
	ll m = a[0].size();
	ll e = b[0].size();
	vector<vector<ll>> res(n, vector<ll>(e, 0));
	for (ll i = 0; i < n; i++)
	{
		for (ll j = 0; j < e; j++)
		{
			for (ll k = 0; k < m; k++)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	std::cout << "Time taken by function matrix_multiplication: " << duration.count() << " milliseconds" << endl;
	return res;
}

void vector_multiplication(vector<vector<ll>> a, vector<vector<ll>> b, vector<vector<ll>> &c, vector<pair<int, int>> indexes)
{
	cout << "thread id: " << this_thread::get_id() << endl;
	for (int idx = 0; idx < indexes.size(); idx++)
	{
		for (int k = 0; k < b.size(); k++)
		{
			int x = indexes[idx].first;
			int y = indexes[idx].second;
			//cout << indexes[idx].first << " " << indexes[idx].second << endl;
			c[x][y] += a[x][k] * b[k][y];
		}
	}
}

//DWORD vector_multiplication(LPVOID lpParam)
//{
//	PMYDATA pDataArray;
//	pDataArray = (PMYDATA)lpParam;
//	//cout << "thread id: " << this_thread::get_id() << endl;
//	for (int idx = 0; idx < pDataArray->indexes.size(); idx++)
//	{
//		for (int k = 0; k < pDataArray->b.size(); k++)
//		{
//			int x = pDataArray->indexes[idx].first;
//			int y = pDataArray->indexes[idx].second;
//			//cout << indexes[idx].first << " " << indexes[idx].second << endl;
//			pDataArray->c[x][y] += pDataArray->a[x][k] * pDataArray->b[k][y];
//		}
//	}
//	return 0;
//}

vector<vector<ll>> matrix_multiplication_parallel(vector<vector<ll>> a, vector<vector<ll>> b, ll NUM_THREADS, vector<vector<pair<int, int>>> indexes)
{
	// a (n, m), b (d, e) => res (n, e)
	auto start = chrono::high_resolution_clock::now();
	ll n = a.size();
	ll m = a[0].size();
	ll e = b[0].size();
	if (NUM_THREADS > n * e)
	{
		cout << "Too many threads" << endl;
		NUM_THREADS = n * e;
	}
	vector<thread> th(NUM_THREADS);
	vector<vector<ll>> c(n, vector<ll>(e, 0));
	/*for (lli i = 0; i < n; i++)
	{
		for (lli j = 0; j < e; j++)
		{
			for (lli k = 0; k < m; k++)
			{
				res[i][j] += a[i][k] * b[k][j];
			}
		}
	}*/
	for (int i = 0; i < NUM_THREADS; i++) th[i] = thread(vector_multiplication, a, b, ref(c), indexes[i]);
	for (int i = 0; i < NUM_THREADS; i++) th[i].join();
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	cout << "Time taken by function: matrix_multiplication_parallel " << duration.count() << " milliseconds" << endl;
	return c;
}

//vector<vector<ll>> matrix_multiplication_parallel_winapi(vector<vector<ll>> a, vector<vector<ll>> b, ll NUM_THREADS, vector<vector<pair<int, int>>> indexes)
//{
//	// a (n, m), b (d, e) => res (n, e)
//	auto start = chrono::high_resolution_clock::now();
//	ll n = a.size();
//	ll m = a[0].size();
//	ll e = b[0].size();
//	if (NUM_THREADS > n * e)
//	{
//		std::cout << "Too many threads" << endl;
//		NUM_THREADS = n * e;
//	}
//	PMYDATA pDataArray[MAX_THREADS];
//	HANDLE th[MAX_THREADS];
//	vector<DWORD> dwThreadIdArray(NUM_THREADS);
//	vector<vector<ll>> c(n, vector<ll>(e, 0));
//	/*for (lli i = 0; i < n; i++)
//	{
//		for (lli j = 0; j < e; j++)
//		{
//			for (lli k = 0; k < m; k++)
//			{
//				res[i][j] += a[i][k] * b[k][j];
//			}
//		}
//	}*/
//	for (int i = 0; i < NUM_THREADS; i++)
//	{
//		pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
//		
//		if (pDataArray[i] == NULL)
//		{
//			std::cout << "exit" << endl;
//			ExitProcess(2);
//		}
//		std::cout << i << endl;
//
//		pDataArray[i]->a = a;
//		pDataArray[i]->b = b;
//		pDataArray[i]->c = c;
//		pDataArray[i]->indexes = indexes[i];
//
//
//		th[i] = CreateThread(
//			NULL,                   // default security attributes
//			0,                      // use default stack size  
//			vector_multiplication,       // thread function name
//			pDataArray[i],          // argument to thread function 
//			0,                      // use default creation flags 
//			&dwThreadIdArray[i]);   // returns the thread identifier 
//
//		if (th[i] == NULL)
//		{
//			std::cout << "couldn't create a thread" << endl;
//			ExitProcess(3);
//		}
//	}
//	//for (int i = 0; i < NUM_THREADS; i++) th[i].join();
//
//	WaitForMultipleObjects(NUM_THREADS, th, TRUE, INFINITE);
//
//	for (int i = 0; i < NUM_THREADS; i++)
//	{
//		CloseHandle(th[i]);
//		if (th[i] != NULL)
//		{
//			HeapFree(GetProcessHeap(), 0, th[i]);
//			th[i] = NULL;    // Ensure address is not reused.
//		}
//	}
//
//	auto stop = chrono::high_resolution_clock::now();
//	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
//	std::cout << "Time taken by function: matrix_multiplication_parallel " << duration.count() << " microseconds" << endl;
//	return c;
//}

void print_matrix(vector<vector<ll>> a)
{
	for (ll i = 0; i < a.size(); i++)
	{
		for (ll j = 0; j < a[0].size(); j++) std::cout << a[i][j] << " ";
		std::cout << endl;
	}
}

void print_matrix_to_file(vector<vector<ll>> a, string name)
{
	ofstream file(name);
	for (ll i = 0; i < a.size(); i++)
	{
		for (ll j = 0; j < a[0].size(); j++) file << a[i][j] << " ";
		file << endl;
	}
	file.close();
}

int main()
{
	//generate_file(200, 400, 400, 800);
	string name = "200x400_400x800";
	int NUM_THREADS;
	cout << "Enter number of threads: ";
	cin >> NUM_THREADS;
	ifstream file(name + ".txt");
	ll n, m;
	file >> n >> m;
	vector<vector<ll>> a(n, vector<ll>(m));
	for (ll i = 0; i < n; i++)
	{
		for (ll j = 0; j < m; j++) file >> a[i][j];
	}
	ll d, e;
	file >> d >> e;
	vector<vector<ll>> b(d, vector<ll>(e));
	for (ll i = 0; i < d; i++)
	{
		for (ll j = 0; j < e; j++) file >> b[i][j];
	}
	if (NUM_THREADS > n * e)
	{
		std::cout << "Too many threads" << endl;
		NUM_THREADS = n * e;
	}
	vector<vector<pair<int, int>>> indexes(NUM_THREADS, vector<pair<int, int>>());
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < e; j++)
		{
			indexes[count % NUM_THREADS].push_back({i, j});
			count++;
		}
	}
	/*for (int i = 0; i < NUM_THREADS; i++)
	{
		cout << i << "-th thread: ";
		for (int j = 0; j < indexes[i].size(); j++)
		{
			cout << "{" << indexes[i][j].first << ", " << indexes[i][j].second << "}" << ", ";
		}
		cout << endl;
	}*/
	vector<vector<ll>> c1 = matrix_multiplication(a, b);
	print_matrix_to_file(c1, name + "_output.txt");
	vector<vector<ll>> c2 = matrix_multiplication_parallel(a, b, NUM_THREADS, indexes);
	print_matrix_to_file(c2, name + "output_parallel.txt");
	//this_thread::sleep_until(chrono::high_resolution_clock::now() + chrono::seconds(1));
	//print_matrix(c1);
	//cout << endl;
	//print_matrix(c2);
	file.close();
	return 0;
}