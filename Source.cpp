#include "DB.h"
#include <stdlib.h>
#include "Controller.h"

using namespace std;

int main(int argc, char* argv[])
{
	Database& db = Database::getInstance(); //63 68 65 70 61
	Controller& ctrl = Controller::getInstance();
	int mainSelect = 99;
	int SecSelect = 0;

	db.getConnection();
	while (mainSelect != 0) {
		cout << "\n\n"
			<< "1) login" << endl
			<< "2) sign up" << endl;
		//<< " RECORD STUDIO-----Please mainSelect one of the options:" << endl
		//<< " 1) Show how much albums recorded between 2 dates." << endl
		//<< " 2) Show how many songs musician recorded between 2 dates." << endl
		//<< " 3) Show how many diffrent albums recorded between 2 dates by specific musician." << endl
		//<< " 4) Show the most popular music instrument." << endl
		//<< " 5) Show instruments in selected album." << endl
		//<< " 6) Most productive producer between 2 dates." << endl
		//<< " 7) Most popular manufacturer between 2 dates." << endl
		//<< " 8) How much musicians recorder in all time." << endl
		//<< " 9) Show most collaborative musician ." << endl
		//<< " 10) Most popular genre over all times." << endl
		//<< " 11) Technician that recorded the biggest number of songs." << endl
		//<< " 12) First album that was recorded in studio." << endl
		//<< " 13) List of songs that included in 2 or more albums." << endl
		//<< " 14) List of technicians that recorded s whole album." << endl
		//<< " 15) Musician that recorded the most songs in different genres." << endl
		//<< " 0) End the program." << endl
		//<< " Your Choice: ";
		cin >> mainSelect;

		switch (mainSelect) {
		case 1:
		{
			if (ctrl.login("s@s.s", "123"))
			{
				cout << "yes";
				SecSelect = 1;
				system("CLS");
			}
			else
			{
				cout << "no";
			}
			break;
		}
		case 2:
		{
			if (ctrl.signUp("bbb@cc.d", "name", "pass"))
			{
				cout << "yes";
				SecSelect = 1;
			}
			else
			{
				cout << "no";
			}
			break;
		}
		default:
		{
			cout << "please select the options above";
		}
		}
		while (SecSelect != 0)
		{
			/*cout.clear();*/
			
			ctrl.welcomeMSG();
			cin >> SecSelect;
			switch (SecSelect)
			{
			case 1:
			{
				ctrl.browse(3, "london", 1, 40);
				cout << endl;
				break;
			}
			case 2:
			{
				ctrl.buy(1);
				break;
			}
			default:
				cout << "please select the options above";
				break;
			}

		}

		//	case 0: {

		//		cout << "Bye!" << endl;
		//		exit(EXIT_SUCCESS);
		//	}

		//	case 1: {
		//		db.showBetweenTwoDates();
		//		cin.clear();
		//		cin.ignore(10000, '\n');
		//		break;
		//	}

		//	case 2: {
		//		db.showSongsBetweenTwoDates();
		//		cin.clear();
		//		cin.ignore(10000, '\n');
		//		break;
		//	}

		//	case 3: {
		//		db.showAlbumsBetweenDatesAndComposer();
		//		cin.clear();
		//		cin.ignore(10000, '\n');
		//		break;
		//	}

		//	case 4: {
		//		db.showMostPopularInstrument();
		//		cin.clear();
		//		cin.ignore(10000, '\n');
		//		break;
		//	}

		//	case 5: {
		//		db.instrumentsInAlbum();
		//		cin.clear();
		//		cin.ignore(10000, '\n');
		//		break;
		//	}

		//	case 6: {
		//		db.releaseMostNumOfAlbums();
		//		cin.clear();
		//		cin.ignore(10000, '\n');
		//		break;
		//	}

		//	case 7: {
		//		db.mostPopularManufacturer();
		//		cin.clear();
		//		break;
		//	}

		//	case 8: {
		//		db.totalRecordsInMin();
		//		cin.clear();
		//		break;
		//	}

		//	case 9: {
		//		db.mostAssistMusician();
		//		cin.clear();
		//		break;
		//	}

		//	case 10: {
		//		db.mostPopularGenre();
		//		cin.clear();
		//		break;
		//	}

		//	case 11: {
		//		db.bestThechnician();
		//		cin.clear();
		//		break;
		//	}
		//	case 12: {
		//		db.firstRecAlbum();
		//		cin.clear();
		//		break;
		//	}

		//	case 13: {
		//		db.songsInTwoOrMoreAlbums();
		//		cin.clear();
		//		break;
		//	}

		//	case 14: {
		//		db.techniciansInWholeAlbum();
		//		cin.ignore(10000, '\n');
		//		break;
		//	}

		//	case 15: {
		//		db.musicianWithMostGenres();
		//		cin.clear();
		//		break;
		//	}

		//	default: {
		//		cout << "\ninvalid choice" << endl;
		//		mainSelect = 42;
		//		cin.clear();
		//		cin.ignore(10000, '\n');
		//		break;
		//	}

		//	}

		//	cout << endl << "Press 1 to Return To the Main Menu.\nYour Choice: " << endl;
		//	mainSelect = 0; //by CHEPA
		//	while (mainSelect != 1) {
		//		cin >> mainSelect;
		//		cin.clear();
		//		cin.ignore(10000, '\n');
		//	}


		//}
	}
	return 0;
}

