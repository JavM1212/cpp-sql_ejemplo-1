#include "db_logic.h"
#include <string>
#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>

using namespace std;

char db::HOST[] = "localhost";
char db::USER[] = "root";
char db::PASS[] = "123456";

MYSQL* db::connectToDatabase(const char* dbName) { 
    MYSQL* obj = mysql_init(0); // inicializar mysql
    if (!obj) {
        cerr << "ERROR al inicializar MySql" << std::endl;
        return 0;
    }

    if (!mysql_real_connect(obj, HOST, USER, PASS, dbName, 3306, NULL, 0)) { // conectarse a la base de datos especifica
        cerr << "ERROR: " << mysql_error(obj) << endl;
        mysql_close(obj);
        return 0;
    }

    return obj;
}

void db::closeDatabase(MYSQL* obj) {
    if (obj) { // cerrar la base de datos
        mysql_close(obj);
    }
}

void db::create_estudiante() {
	MYSQL *obj = db::connectToDatabase("universidad");

	// fields para ingresar para un registro
    char Carne[7]; // aunque en la db esta de tamano 6 aca se pone de tamano 7 porque al final tiene un caracter de salto de linea por el getline, ese caracter no se guarda en la db
    char Nombre[21];
    char FechaNacimiento[11]; // "yyyy-mm-dd"
    float Ponderado;
    
    char* consulta;
    char* consulta_sql;
    string consulta_sql_aux;

    if (!obj) {
        return;
    } else {
    	// obtener los valores de los fields
        cout << "Carne: ";
        cin.getline(Carne, 7, '\n');

        cout << "Nombre: ";
        cin.getline(Nombre, 21, '\n');

        cout << "Fecha de Nacimiento: ";
        cin.getline(FechaNacimiento, 11, '\n');

        cout << "Ponderado: ";
        cin >> Ponderado;
        cin.ignore(100, '\n'); // limpiar terminal en input de valor numerico
        
        consulta_sql_aux = "INSERT INTO estudiante(Carne, Nombre, FechaNacimiento, Ponderado) VALUES('%s', '%s', '%s', '%f')"; // consulta que luego se remplazaran los valores
        consulta_sql = new char[consulta_sql_aux.length() + 1];
        strcpy(consulta_sql, consulta_sql_aux.c_str()); // basicamente pasar la consulta de sql de string a *char

        consulta = new char[strlen(consulta_sql) + strlen(Carne) + strlen(Nombre) + strlen(FechaNacimiento) + sizeof(float)]; // apartar el espacio necesario dinamicamente
        sprintf(consulta, consulta_sql, Carne, Nombre, FechaNacimiento, Ponderado); // remplaza las variables con % como '%s' con las variables pasadas, el orden importa

        if (mysql_ping(obj)) {
            cout << "ERROR: " << mysql_error(obj);
        }

        if (mysql_query(obj, consulta)) {
            cout << "ERROR: " << mysql_error(obj) << endl;
            rewind(stdin); // otra forma de limpiar consola
            getchar(); // pedir un char para continuar
        } else {
            cout << endl << "Estudiante agregado correctamente" << endl << endl;
            mysql_store_result(obj); // guardar en la db
        }
    }
    
    db::closeDatabase(obj);
}

void db::read_estudiantes() {
	MYSQL *obj = db::connectToDatabase("universidad");
	MYSQL_ROW row; // donde se van a almacenar los registros
	
    if (!obj) {
        return;
    } else {
        if (mysql_ping(obj)) {
            cout << "ERROR: " << mysql_error(obj);
        }

        if (mysql_query(obj, "SELECT * FROM estudiante")) { // realizar la query, la funcion devuelve un code en formato int, si es diferente de 0 da error y si da 0 fue exitosa
            cout << "ERROR: " << mysql_error(obj) << endl;
            rewind(stdin);
            getchar();
        } else {
            cout << "Estudiantes actuales: " << endl;
            
            MYSQL_RES *result = mysql_store_result(obj);
            while ((row = mysql_fetch_row(result)) != NULL) { // mientras no devuelva null va a traer rows
                for (int i = 0; i < mysql_num_fields(result); i++) { // cada row es un array de fields, por decirlo asi, cada vez que trae una se puede ver como row = [carne, nombre, fecha, ponderado], por lo que el for recorre cada field
                    cout << row[i] << " "; // se imprime el field
                }
                cout << endl;
            }
            
            cout << endl;
		}
    }
    
	db::closeDatabase(obj); // cerrar la db
}

void db::update_estudiante() {
    MYSQL *obj = db::connectToDatabase("universidad");
    char Carne[7];
    float new_ponderado;

    if (!obj) {
        return;
    } else {
        cout << "Carne: "; 
        cin.getline(Carne, sizeof(Carne), '\n'); // se elimina por carne por ser unico
        cout << "Ponderado: "; // se modifica el ponderado porque es lo unico que tiene sentido cambiar, el nombre, fecha de nacimiento o carne no cambian
        cin >> new_ponderado;
        cin.ignore(100, '\n');

        char new_ponderado_str[20];  // se pone un tamano maximo
        sprintf(new_ponderado_str, "%f", new_ponderado);

        string query = "UPDATE estudiante SET Ponderado = " + string(new_ponderado_str) + " WHERE Carne = '" + string(Carne) + "'"; // se construye la query pasando a string las variables

        if (mysql_query(obj, query.c_str())) { // se realiza la query
            cout << "ERROR: " << mysql_error(obj) << endl;
        } else {
            cout << endl << "Registro modificado exitosamente" << endl << endl;
        }
    }
    
    db::closeDatabase(obj);
}

void db::delete_estudiante() {
    MYSQL *obj = db::connectToDatabase("universidad");
    char Carne[7];

    if (!obj) {
        cout << "ERROR: " << endl;
        return;
    } else {    
        cout << "Carne: ";
        cin.getline(Carne, sizeof(Carne), '\n'); // se elimina por carne por ser unico

        string query = "DELETE FROM estudiante WHERE Carne = '" + string(Carne) + "'"; // se construye la query

        if (mysql_query(obj, query.c_str())) { // se ejecuta la query
            cout << "ERROR: " << mysql_error(obj) << endl;
        } else {
            cout << endl << "Registro eliminado exitosamente!" << endl << endl;
        }
    }
    
    db::closeDatabase(obj);
}
