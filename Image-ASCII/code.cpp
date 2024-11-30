#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <conio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

const string ASCII_CHARS = "@%#*+=-:. ";

int minInt(int a, int b) { 
    
    return (a < b) ? a : b; 
    
}

int maxInt(int a, int b) { 
    
    return (a > b) ? a : b; 
    
}

void waitForUser() {

    cout << "\nPress any key to continue..." << endl;
    
    _getch();
}

void displayImageMetadata(int width, int height, int channels) {

    cout << "\n==========================================" << endl;
    cout << "            IMAGE METADATA                " << endl;
    cout << "==========================================" << endl;
    cout << "Width       : " << width << " px" << endl;
    cout << "Height      : " << height << " px" << endl;
    cout << "Channels    : " << channels << " (1=Grayscale, 3=RGB, 4=RGBA)" << endl;
    cout << "==========================================" << endl;

    waitForUser();
}

class Node {
public:
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {}
};

class LinkedList {
public:
    Node* head;

    LinkedList() : head(nullptr) {}

    ~LinkedList() {

        while (head != nullptr) {

            Node* temp = head;

            head = head->next;

            delete temp;
        }
    }

    void push_back(int value) {

        Node* newNode = new Node(value);

        if (!head) {

            head = newNode;

        } else {

            Node* temp = head;

            while (temp->next) {

                temp = temp->next;

            }

            temp->next = newNode;
        }
    }

    Node* pop_front() {

        if (!head) return nullptr;

        Node* temp = head;

        head = head->next;

        return temp;
    }

    bool isEmpty() {
        return head == nullptr;
    }
};
class Queue  
{
public:
    Node* front;
    Node* rear;
    Queue() : front(nullptr), rear(nullptr) {}
    ~Queue()
    {
        while (front != nullptr)
        {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }
    bool isEmpty()
    {
        return front == nullptr;
    }
    void enqueue(int value)
    {
        Node* newNode = new Node(value);
        if (rear)
        {
            rear->next = newNode;
        }
        else
        {
            front = newNode;
        }
        rear = newNode;
    }
    int dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue is empty!" << endl;
            return -1;
        }
        Node* temp = front;
        int value = front->data;
        front = front->next;
        if (front == nullptr)
        {
            rear = nullptr;
        }
        delete temp;
        return value;
    }
    int peek()
    {
        if (isEmpty())
        {
            cout << "Queue is empty!" << endl;
            return -1;
        }
        return front->data;
    }
    void print()
    {
        Node* temp = front;
        while (temp)
        {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};
class PriorityQueue {

public:
    LinkedList list;

    void push(int priority, int data) {

        Node* newNode = new Node(priority);
        newNode->next = new Node(data);

        if (list.isEmpty()) {

            list.head = newNode;

            return;
        }

        Node* current = list.head;

        if (current->data > priority) {

            newNode->next = list.head;

            list.head = newNode;

            return;
        }

        while (current->next && current->next->data <= priority) {

            current = current->next;

        }

        newNode->next = current->next;

        current->next = newNode;
    }

    Node* pop() {

        return list.pop_front();
    }

    bool isEmpty() {

        return list.isEmpty();
    }
};

class Graph {

public:
    int width, height;
    LinkedList* adjList;

    Graph(int w, int h) : width(w), height(h) {

        adjList = new LinkedList[w * h];
    }

    ~Graph() {

        delete[] adjList;
    }

    void addEdge(int u, int v) {

        adjList[u].push_back(v);

        adjList[v].push_back(u);
    }

    vector<int> dijkstra(int start, unsigned char* image) {

        int n = width * height;

        vector<int> dist(n, INT_MAX);

        vector<bool> visited(n, false);

        PriorityQueue pq;

        dist[start] = 0;

        pq.push(0, start);

        while (!pq.isEmpty()) {

            Node* currentNode = pq.pop();

            int current = currentNode->next->data;

            if (visited[current]) continue;

            visited[current] = true;

            for (Node* neighbor = adjList[current].head; neighbor != nullptr; neighbor = neighbor->next) {

                int weight = abs(image[current] - image[neighbor->data]);

                if (dist[current] + weight < dist[neighbor->data]) {

                    dist[neighbor->data] = dist[current] + weight;

                    pq.push(dist[neighbor->data], neighbor->data);
                }
            }
        }

        return dist;
    }
};

void convertToASCII(unsigned char* image, int width, int height) {

    ofstream outFile("ascii_art.txt");

    if (!outFile) {

        cout << "\nError opening file to save ASCII art!" << endl;

        return;
    }

    Graph graph(width, height);

    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            int grayValue = image[y * width + x];

            int index = grayValue * (ASCII_CHARS.size() - 1) / 255;

            if (x + 1 < width) {

                int rightPixel = y * width + (x + 1);

                graph.addEdge(y * width + x, rightPixel);
            }

            if (y + 1 < height) {

                int bottomPixel = (y + 1) * width + x;

                graph.addEdge(y * width + x, bottomPixel);
            }

            outFile << ASCII_CHARS[index];
        }

        outFile << endl;
    }

    cout << "\nASCII art successfully generated and saved as 'ascii_art.txt'!" << endl;

    waitForUser();
}

void adjustBrightnessGreedy(unsigned char* image, int width, int height) {

    int adjustment = 0;

    for (int i = 0; i < width * height; i++) {

        adjustment = min(adjustment, 255 - image[i]);

        adjustment = max(adjustment, -image[i]);
    }

    for (int i = 0; i < width * height; i++) {

        image[i] += adjustment;
    }

    cout << "\nBrightness adjusted by " << adjustment << " levels.\n";

    waitForUser();
}

void detectEdgesDP(unsigned char* image, int width, int height) {
    vector<vector<int>> dp(height, vector<int>(width, 0));
    ofstream outFile("edge_detection_output.txt");

    if (!outFile) {
        cout << "\nError opening file to save edge detection output!" << endl;
        return;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int gx = 0, gy = 0;

            if (x > 0)
                gx += abs(image[y * width + x] - image[y * width + (x - 1)]);

            if (y > 0)
                gy += abs(image[y * width + x] - image[(y - 1) * width + x]);

            dp[y][x] = gx + gy;

            outFile << dp[y][x] << " ";
        }
        outFile << endl; 
    }

    outFile.close();

    cout << "\nEdge detection completed. Output saved as 'edge_detection_output.txt'.\n";

    waitForUser();
}


void displayAbout() {

    cout << "\n==========================================" << endl;
    cout << "                ABOUT                     " << endl;
    cout << "==========================================" << endl;
    cout << "Project Name: Image To ASCII Conversion" << endl;
    cout << "Features    : ASCII Conversion, Brightness Adjustment,\n";
    cout << "              Edge Detection, and Dijkstra's Shortest Path." << endl;
    cout << "Created By  : Parth Khare, Arnav Vats,Shivek Gupta." << endl;
    cout << "==========================================" << endl;

    waitForUser();
}

void showMenu(unsigned char* image, int width, int height, int channels) {

    while (true) {

        cout << "\n==========================================" << endl;
        cout << "              MAIN MENU                   " << endl;
        cout << "==========================================" << endl;
        cout << "1. Display Image Metadata" << endl;
        cout << "2. Convert Image to ASCII Art" << endl;
        cout << "3. Adjust Brightness " << endl;
        cout << "4. Detect Edges " << endl;
        cout << "5. About the Project" << endl;
        cout << "6. Exit" << endl;
        cout << "==========================================" << endl;
        cout << "Enter your choice: ";

        int choice;

        cin >> choice;

        switch (choice) {

            case 1:
                displayImageMetadata(width, height, channels);
                break;

            case 2:
                convertToASCII(image, width, height);
                break;

            case 3:
                adjustBrightnessGreedy(image, width, height);
                break;

            case 4:
                detectEdgesDP(image, width, height);
                break;

            case 5:
                displayAbout();
                break;

            case 6:
                cout << "\nExiting program..." << endl;
                return;

            default:
                cout << "\nInvalid choice. Please try again." << endl;
        }
    }
}

void loadImage(unsigned char*& image, int& width, int& height, int& channels) {

    string filename;

    cout << "\nEnter the image filename (with extension): ";
    cin >> filename;

    image = stbi_load(filename.c_str(), &width, &height, &channels, 1);
    if (!image) {
        cout << "\nError loading image!" << endl;
        return;
    }
    
    cout << "\nImage loaded successfully!" << endl;
    displayImageMetadata(width, height, channels);
}
int main() {

    int width, height, channels;

    unsigned char* image = nullptr;
    loadImage(image, width, height, channels);
    if (!image) {
        return 1;
    }

    showMenu(image, width, height, channels);

    stbi_image_free(image);

return 0;
}