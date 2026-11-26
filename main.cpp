#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

// Book class
class Book {
private:
    int id;
    string title;
    string author;

public:
    Book(int i, string t, string a) {
        id = i;
        title = t;
        author = a;
    }

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }

    void display() const {
        cout << left << setw(8) << id
             << setw(25) << title
             << author << endl;
    }
};

// Member class
class Member {
private:
    int id;
    string name;

public:
    Member(int i, string n) {
        id = i;
        name = n;
    }

    int getId() const { return id; }
    string getName() const { return name; }
};

// Loan class
class Loan {
private:
    int bookId;
    int memberId;
    bool active;

public:
    Loan(int b, int m, bool a = true) {
        bookId = b;
        memberId = m;
        active = a;
    }

    int getBookId() const { return bookId; }
    int getMemberId() const { return memberId; }
    bool isActive() const { return active; }

    void returnBook() {
        active = false;
    }
};

// Library class
class Library {
private:
    vector<Book> books;
    vector<Member> members;
    vector<Loan> loans;

    const string filename = "library_data.txt";

    int findBook(int id) const {
        for (int i = 0; i < (int)books.size(); i++) {
            if (books[i].getId() == id)
                return i;
        }
        return -1;
    }

    int findMember(int id) const {
        for (int i = 0; i < (int)members.size(); i++) {
            if (members[i].getId() == id)
                return i;
        }
        return -1;
    }

    bool isAvailable(int bookId) const {
        for (const Loan& loan : loans) {
            if (loan.getBookId() == bookId && loan.isActive())
                return false;
        }
        return true;
    }

public:
    // Load saved records when the program starts
    void loadData() {
        ifstream file(filename);

        if (!file.is_open())
            return;

        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string type;

            getline(ss, type, '|');

            if (type == "B") {
                string idText, title, author;
                getline(ss, idText, '|');
                getline(ss, title, '|');
                getline(ss, author);

                try {
                    books.emplace_back(stoi(idText), title, author);
                } catch (...) {}
            }
            else if (type == "M") {
                string idText, name;
                getline(ss, idText, '|');
                getline(ss, name);

                try {
                    members.emplace_back(stoi(idText), name);
                } catch (...) {}
            }
            else if (type == "L") {
                string bookText, memberText, activeText;
                getline(ss, bookText, '|');
                getline(ss, memberText, '|');
                getline(ss, activeText);

                try {
                    loans.emplace_back(
                        stoi(bookText),
                        stoi(memberText),
                        activeText == "1"
                    );
                } catch (...) {}
            }
        }

        file.close();
    }

    // Save all records to a local file
    void saveData() const {
        ofstream file(filename);

        for (const Book& book : books) {
            file << "B|" << book.getId() << "|"
                 << book.getTitle() << "|"
                 << book.getAuthor() << "\n";
        }

        for (const Member& member : members) {
            file << "M|" << member.getId() << "|"
                 << member.getName() << "\n";
        }

        for (const Loan& loan : loans) {
            file << "L|" << loan.getBookId() << "|"
                 << loan.getMemberId() << "|"
                 << (loan.isActive() ? 1 : 0) << "\n";
        }

        file.close();
    }

    void addBook() {
        int id;
        string title, author;

        cout << "Enter book ID: ";
        if (!(cin >> id) || id <= 0) {
            cout << "Invalid book ID.\n";
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (findBook(id) != -1) {
            cout << "Book ID already exists.\n";
            return;
        }

        cout << "Enter book title: ";
        getline(cin, title);

        cout << "Enter author name: ";
        getline(cin, author);

        if (title.empty() || author.empty()) {
            cout << "Title and author cannot be empty.\n";
            return;
        }

        books.emplace_back(id, title, author);
        saveData();

        cout << "Book added successfully!\n";
    }

    void addMember() {
        int id;
        string name;

        cout << "Enter member ID: ";
        if (!(cin >> id) || id <= 0) {
            cout << "Invalid member ID.\n";
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (findMember(id) != -1) {
            cout << "Member ID already exists.\n";
            return;
        }

        cout << "Enter member name: ";
        getline(cin, name);

        if (name.empty()) {
            cout << "Name cannot be empty.\n";
            return;
        }

        members.emplace_back(id, name);
        saveData();

        cout << "Member added successfully!\n";
    }

    void displayBooks() const {
        if (books.empty()) {
            cout << "No books found.\n";
            return;
        }

        cout << "\nID      Title                    Author\n";
        cout << "---------------------------------------------\n";

        for (const Book& book : books) {
            book.display();
            cout << "        Status: "
                 << (isAvailable(book.getId()) ? "Available" : "Issued")
                 << "\n";
        }
    }

    void searchBook() const {
        int id;
        cout << "Enter book ID to search: ";

        if (!(cin >> id)) {
            cout << "Invalid ID.\n";
            return;
        }

        int index = findBook(id);

        if (index == -1) {
            cout << "Book not found.\n";
            return;
        }

        books[index].display();

        cout << "Status: "
             << (isAvailable(id) ? "Available" : "Issued") << "\n";
    }

    void issueBook() {
        int bookId, memberId;

        cout << "Enter book ID: ";
        if (!(cin >> bookId)) {
            cout << "Invalid book ID.\n";
            return;
        }

        cout << "Enter member ID: ";
        if (!(cin >> memberId)) {
            cout << "Invalid member ID.\n";
            return;
        }

        if (findBook(bookId) == -1) {
            cout << "Book not found.\n";
            return;
        }

        if (findMember(memberId) == -1) {
            cout << "Member not found. Add the member first.\n";
            return;
        }

        if (!isAvailable(bookId)) {
            cout << "This book is already issued.\n";
            return;
        }

        loans.emplace_back(bookId, memberId);
        saveData();

        cout << "Book issued successfully!\n";
    }

    void returnBook() {
        int bookId, memberId;

        cout << "Enter book ID: ";
        if (!(cin >> bookId)) {
            cout << "Invalid book ID.\n";
            return;
        }

        cout << "Enter member ID: ";
        if (!(cin >> memberId)) {
            cout << "Invalid member ID.\n";
            return;
        }

        for (Loan& loan : loans) {
            if (loan.getBookId() == bookId &&
                loan.getMemberId() == memberId &&
                loan.isActive()) {

                loan.returnBook();
                saveData();

                cout << "Book returned successfully!\n";
                return;
            }
        }

        cout << "No active loan found for these details.\n";
    }
};

// Main function
int main() {
    Library library;
    library.loadData();

    int choice;

    do {
        cout << "\n========== LIBRARY MANAGER ==========\n";
        cout << "1. Add Book\n";
        cout << "2. Add Member\n";
        cout << "3. Display All Books\n";
        cout << "4. Search Book\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.addMember();
                break;
            case 3:
                library.displayBooks();
                break;
            case 4:
                library.searchBook();
                break;
            case 5:
                library.issueBook();
                break;
            case 6:
                library.returnBook();
                break;
            case 7:
                cout << "Thank you for using Library Manager!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 7);

    return 0;
}