#pragma once

template <typename T>
class List{
	struct Elem{
		T pod;
		Elem* next;
		Elem(T p,Elem* s=0){
			pod=p;
			next=s;
		}
	};
	Elem* prvi,*posl;
	Elem* temp;
public:
	List();
	~List();
	void put(T p); //dodavanje na kraj
	void putInFront(T p);
	T getFirst();
	void remove(T p);
	bool find(T s);
	T get(T s); //vraca 0 ako ne nadje
	T& getByRef(T s); //vraca po referenci
	bool hasNext();
	void onNext();
	void onBegin();
	void onNumber(int n);
	void insertAfter(T s, int n);
	T& getCurrent(); //vraca podatak na koji ukazuje temp!
	T getCurrentByValue();
	T findByNumber(int n);
	bool isEmpty();
	void update(T s);
};

template <typename T>
List<T>::List(){
	prvi = posl = temp = 0;
}

template <typename T>
List<T>::~List(){
	Elem* tek = prvi, *stari;
	while (tek){
		stari = tek;
		tek = tek->next;
		delete stari;
	}
	delete tek;
}

template <typename T>
void List<T>::put(T p){
	Elem* novi = new Elem(p);
	if (!prvi)
		prvi = novi;
	else
		posl->next = novi;
	posl = novi;

	temp = prvi;
}
template <typename T>
bool List<T>::isEmpty(){
	if (!prvi)
		return true;
	return false;
}

template <typename T>
void List<T>::putInFront(T p){
	Elem* novi = new Elem(p);
	if (prvi){
		novi->next = prvi;
		prvi = novi;
	}
	else{
		prvi = posl = novi;
	}
	temp = prvi;
}

template <typename T>
T List<T>::getFirst(){
	if (prvi){
		Elem* stari = prvi;
		prvi = prvi->next;

		temp = prvi;
		T p = stari->pod;
		delete stari;
		return p;
	}
	else
		return 0;
}

template <typename T>
void List<T>::remove(T p){
	Elem* tek = prvi, *pret = 0;
	while (tek && tek->pod == p){
		pret = tek;
		tek = tek->next;
	}
	if (tek){
		if (!pret)
			prvi = prvi->next;
		else
			pret->next = tek->next;
		if (!tek->next)
			posl = pret;
		if (!prvi)
			posl = 0;

		temp = prvi;
	}
}

template <typename T>
bool List<T>::find(T s){
	Elem* tek = prvi;
	while (tek){
		if (tek->pod == s){
			return true;
		}
		tek = tek->next;
	}
	return false;
}

template <typename T>
void List<T>::update(T s){
	Elem* tek = prvi;
	while (tek){
		if (tek->pod == s){
			tek->pod = s;
			break;
		}
		tek = tek->next;
	}
}

template <typename T>
T List<T>::get(T s){
	Elem* tek = prvi;
	while (tek){
		if (tek->pod == s){
			return tek->pod;
		}
		tek = tek->next;
	}
	return tek->pod;
}

template <typename T>
T& List<T>::getByRef(T s){
	Elem* tek = prvi;
	while (tek){
		if (tek->pod == s){
			return tek->pod;
		}
		tek = tek->next;
	}
	return tek->pod;
}

template <typename T>
bool List<T>::hasNext(){
	if (!temp)
		return false;

	if (temp->next!=0)
		return true;

	return false;
}

template <typename T>
void List<T>::onNext(){ //uvek pisati u paru sa hasNext()!
	temp = temp->next;
}

template <typename T>
void List<T>::onBegin(){
	if (prvi)
		temp = prvi;
	else
		temp = 0;
}

template <typename T>
T& List<T>::getCurrent(){
	return temp->pod;
}

template <typename T>
T List<T>::getCurrentByValue(){
	return temp->pod;
}

template <typename T>
void List<T>::onNumber(int n){
	Elem* tek = prvi;
	int i = 0;
	while (tek){
		if (n == i){
			temp = tek;
			return;
		}
		tek = tek->next;
		i++;
	}
}

template <typename T>
T List<T>::findByNumber(int n){
	Elem* tek = prvi;
	int i = 0;
	while (tek){
		if (n == i){
			return tek->pod;
		}
		i++;
		tek = tek->next;
	}
	return tek->pod; //ne bi trebalo da dodje ovde
}

template <typename T>
void List<T>::insertAfter(T s, int n){
	if (!prvi){
		prvi = posl=new Elem(s);
		temp = prvi;
		return;
	}
	Elem* tek = prvi;
	int i = 0;
	while (tek){
		if (n == i){
			Elem* novi = new Elem(s);
			if (tek->next)
				novi->next = tek->next;
			else
				posl = novi;
			tek->next = novi;
			return;
		}
		i++;
		tek = tek->next;
	}
}
