// Lab 4 - Task 1: Playlist Manager using Doubly Linked List

#include <iostream>
#include <string>
using namespace std;

// one song = one node
struct Song
{
    int id;
    string title;
    int mins, secs;
    Song *prev;
    Song *next;

    Song(int i, string t, int m, int s)
    {
        id = i; title = t; mins = m; secs = s;
        prev = NULL; next = NULL;
    }
};

class Playlist
{
    Song *head;
    Song *tail;
    Song *nowPlaying; // pointer used for next / previous

public:
    Playlist() { head = tail = nowPlaying = NULL; }

    ~Playlist() // free all memory
    {
        Song *t = head;
        while (t != NULL)
        {
            Song *rem = t;
            t = t->next;
            delete rem;
        }
    }

    Song* findSong(int id)
    {
        Song *t = head;
        while (t != NULL)
        {
            if (t->id == id) return t;
            t = t->next;
        }
        return NULL;
    }

    void printSong(Song *s)
    {
        cout << "  ID: " << s->id << " | " << s->title << " | "
             << s->mins << ":" << (s->secs < 10 ? "0" : "") << s->secs << endl;
    }

    // 1. add at the end
    void addSong(int id, string title, int m, int s)
    {
        if (findSong(id) != NULL)
        {
            cout << "A song with this ID already exists!\n";
            return;
        }
        Song *n = new Song(id, title, m, s);
        if (head == NULL)
        {
            head = tail = nowPlaying = n;
        }
        else
        {
            tail->next = n;
            n->prev = tail;
            tail = n;
        }
        cout << "Song added.\n";
    }

    // 2. delete by id
    void deleteSong(int id)
    {
        Song *d = findSong(id);
        if (d == NULL) { cout << "Song not found.\n"; return; }

        if (nowPlaying == d) // move current pointer away
        {
            nowPlaying = (d->next != NULL) ? d->next : d->prev;
        }

        if (d == head) head = d->next;
        else d->prev->next = d->next;

        if (d == tail) tail = d->prev;
        else d->next->prev = d->prev;

        delete d;
        cout << "Song deleted.\n";
    }

    // 3. forward display
    void showForward()
    {
        if (head == NULL) { cout << "Playlist is empty.\n"; return; }
        cout << "--- Playlist (First to Last) ---\n";
        for (Song *t = head; t != NULL; t = t->next) printSong(t);
    }

    // 4. backward display
    void showBackward()
    {
        if (tail == NULL) { cout << "Playlist is empty.\n"; return; }
        cout << "--- Playlist (Last to First) ---\n";
        for (Song *t = tail; t != NULL; t = t->prev) printSong(t);
    }

    // 5. search
    void searchSong(int id)
    {
        Song *f = findSong(id);
        if (f == NULL) cout << "Song not found.\n";
        else { cout << "Song found:\n"; printSong(f); }
    }

    // 6. play next / previous
    void playCurrent()
    {
        if (nowPlaying == NULL) { cout << "Nothing to play.\n"; return; }
        cout << "Now playing:\n";
        printSong(nowPlaying);
    }

    void playNext()
    {
        if (nowPlaying == NULL) { cout << "Playlist is empty.\n"; return; }
        if (nowPlaying->next == NULL) { cout << "This is the last song.\n"; return; }
        nowPlaying = nowPlaying->next;
        playCurrent();
    }

    void playPrev()
    {
        if (nowPlaying == NULL) { cout << "Playlist is empty.\n"; return; }
        if (nowPlaying->prev == NULL) { cout << "This is the first song.\n"; return; }
        nowPlaying = nowPlaying->prev;
        playCurrent();
    }

    // 7. reverse in place by swapping prev/next of every node
    void reversePlaylist()
    {
        if (head == NULL) { cout << "Playlist is empty.\n"; return; }
        Song *t = head;
        Song *temp = NULL;
        while (t != NULL)
        {
            temp = t->prev;
            t->prev = t->next;
            t->next = temp;
            t = t->prev; // old next
        }
        temp = head; // swap head and tail
        head = tail;
        tail = temp;
        cout << "Playlist reversed.\n";
    }
};

int main()
{
    Playlist p;
    int choice;

    do
    {
        cout << "\n===== PLAYLIST MENU =====\n";
        cout << "1. Add Song\n2. Delete Song\n3. Display Forward\n4. Display Backward\n";
        cout << "5. Search Song\n6. Play Next\n7. Play Previous\n8. Reverse Playlist\n";
        cout << "9. Show Current Song\n0. Exit\nEnter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            int id, m, s; string name;
            cout << "Song ID: "; cin >> id;
            cout << "Song name: "; cin.ignore(); getline(cin, name);
            cout << "Duration (min sec): "; cin >> m >> s;
            p.addSong(id, name, m, s);
        }
        else if (choice == 2)
        {
            int id; cout << "Enter ID to delete: "; cin >> id;
            p.deleteSong(id);
        }
        else if (choice == 3) p.showForward();
        else if (choice == 4) p.showBackward();
        else if (choice == 5)
        {
            int id; cout << "Enter ID to search: "; cin >> id;
            p.searchSong(id);
        }
        else if (choice == 6) p.playNext();
        else if (choice == 7) p.playPrev();
        else if (choice == 8) p.reversePlaylist();
        else if (choice == 9) p.playCurrent();
        else if (choice != 0) cout << "Invalid choice.\n";

    } while (choice != 0);

    cout << "Goodbye - Mohammad Haroon Ahmadzai (578328)\n";
    return 0;
}
