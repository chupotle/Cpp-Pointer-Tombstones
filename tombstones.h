/////////////////////////////////////////////////////////////////////////////
// tombstones.h, expected interface for CS254 assignment 5
/////////////////////////////////////////////////////////////////////////////

#if !defined(__TOMBSTONES_H__)
#define __TOMBSTONES_H__
#include <exception>
using namespace std;

template <class T> class Pointer;
template <class T> void free(Pointer<T>& obj);

template <class T>
struct tombstone{
	T* realPointer;
	int refCount;
	bool isFree;
};

struct danglingPointerException : public exception{
	const char* what() const throw(){
		return "Dangling Pointer";
	}
};

struct memoryLeakException : public exception{
	const char* what() const throw(){
		return "Memory Leak";
	}
};

template <class T>
class Pointer{
private:
	tombstone<T>* tmbStone;
public:
	void dumpInfo(string oper) const{
		cout << "freed? - " << (tmbStone->isFree) << "\n";
		cout << "reference count - " << (tmbStone->refCount) << "\n";
		cout << "operation - "<<oper<<"\n";
	}

	Pointer<T>(){
		tmbStone = new tombstone<T>;
		tmbStone->realPointer = 0;
		tmbStone->refCount = 0;
		tmbStone->isFree = false;
	}// default constructor

	Pointer<T>(Pointer<T>& p){
		if(p.tmbStone->isFree){
			dumpInfo("copy contructor");
			throw danglingPointerException();
		}
		bool newTmb = false;
		if(!tmbStone){
			newTmb=true;
		}
		tmbStone = p.tmbStone;
		tmbStone->refCount++;
		if(newTmb){
			tmbStone->refCount=1;
		}
	}// copy constructor

	Pointer<T>(T* point){
		tmbStone = new tombstone<T>;
		tmbStone->realPointer = point;
		tmbStone->refCount = 1;
		tmbStone->isFree = false;
	}// bootstrapping constructor

	~Pointer<T>(){
		tmbStone->refCount--;
		if(tmbStone->realPointer && tmbStone->refCount == 0 && !tmbStone->isFree){
			dumpInfo("deconstructor");
			throw memoryLeakException();
		}
		if(tmbStone->refCount == 0 && tmbStone->isFree)
			delete(tmbStone);
	}// destructor

	T& operator*() const{
		if(tmbStone->realPointer && !tmbStone->isFree){
			return *tmbStone->realPointer;
		}
		else{
			dumpInfo("dereferencing");
			throw danglingPointerException();
		}
	}// deferencing

	T* operator->() const{
		if(tmbStone->realPointer && !tmbStone->isFree){
			return tmbStone->realPointer;
		}
		else{
			dumpInfo("field dereferencing");
			throw danglingPointerException();
		}
	}// field dereferencing

	Pointer<T>& operator=(const Pointer<T>& p){
		if(p.tmbStone == tmbStone)
			return *this;
		tmbStone->refCount--;
		if(tmbStone->realPointer && *tmbStone->realPointer != 0 && tmbStone->refCount == 0 && !tmbStone->isFree){
			dumpInfo("assignment");
			throw memoryLeakException();
		}
		tmbStone = p.tmbStone;
		tmbStone->refCount++;
	}// assignment

	friend void free(Pointer<T>& pointer){
		if(pointer.tmbStone->isFree){
			pointer.dumpInfo("freeing");
			throw danglingPointerException();
		}
		free(&*pointer);
		pointer.tmbStone->isFree = true;
	}// delete pointed-at object

	// equality comparisons:
	bool operator==(const Pointer<T>& p) const{
		if(tmbStone->realPointer && !tmbStone->isFree){
			return tmbStone->realPointer == p.tmbStone->realPointer;
		}
		else{
			dumpInfo("==");
			throw danglingPointerException();
		}
	}
	bool operator!=(const Pointer<T>& p) const{
		if(!tmbStone->isFree)
			return tmbStone->realPointer != p.tmbStone->realPointer;
		else{
			dumpInfo("!=");
			throw danglingPointerException();
		}
	}
	bool operator==(const int num) const{ return num == 0 && !tmbStone->realPointer; }
	bool operator!=(const int num) const{ return !(num == 0); }
};
template <class T>
bool operator==(const int n, const Pointer<T>& t) { return t == n; }
template <class T>
bool operator!=(const int n, const Pointer<T>& t) { return t != n; }
#endif // __TOMBSTONES_H__
