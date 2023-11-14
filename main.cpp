#include "db_logic.h"
#include <iostream>

using namespace std;

void menu_logic();


int main() {
	menu_logic();
}

void display_options() {
	cout << "Universidad CRUD" << endl;
    cout << "1. Create Estudiante" << endl;
    cout << "2. Read Estudiantes" << endl;
    cout << "3. Read Estudiante" << endl;
    cout << "4. Update Estudiante" << endl;
    cout << "5. Delete Estudiante" << endl;
    cout << "6. Exit" << endl;
}

void menu_logic() {
	db DB;
	int op;
	
	do {
		display_options();
		cout << "Ingrese su eleccion: ";
		cin >> op;
		cout << endl;
		cin.ignore(100, '\n');
		
		switch(op) {
			case 1:
				DB.create_estudiante();
				break;
			case 2:
				DB.read_estudiantes();
				break;
			case 3:
				DB.read_estudiante();
				break;
			case 4:
				DB.update_estudiante();
				break;
			case 5:
				DB.delete_estudiante();
				break;
			case 6:
				cout << "Saliendo de la DB" << endl;
			default:
				cout << "Opcion invalida, intente de nuevo: ";
		}
	} while (op != 6);
}

