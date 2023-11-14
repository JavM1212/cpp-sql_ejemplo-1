#include <mysql.h>
 
class db {
	private:
		static char HOST[];
    	static char USER[];
    	static char PASS[];
	
	public:
		static MYSQL* connectToDatabase(const char* dbName);
    	static void closeDatabase(MYSQL* obj);
		
		void create_estudiante();	// C (create)
		void read_estudiantes();	// R (read)
		void read_estudiante();	// R (read)
		void update_estudiante();	// U (update)
		void delete_estudiante();	// D (delete)
};
