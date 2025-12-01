#include "crow.h"
#include <vector>
#include <string>
#include <deque>
#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>
#include <iomanip>

std::string generateID() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    std::stringstream ss;
    for (int i = 0; i < 8; i++) {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

struct Node {
    std::string id;
    int value;
    Node* next = nullptr;
    Node* prev = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    std::vector<Node*> forward;
    int maxLvl = 0;

    Node(int val) : value(val), id(generateID()) {}
};

struct Stack {
    std::vector<Node*> items;

    void add(int val) {
        items.push_back(new Node(val));
    }

    void remove() {
        if (!items.empty()) {
            delete items.back();
            items.pop_back();
        }
    }

    void removeValue(int val) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((*it)->value == val) {
                delete* it;
                items.erase(it);
                return;
            }
        }
    }

    void clear() {
        for (auto n : items) delete n;
        items.clear();
    }

    crow::json::wvalue toJson() {
        crow::json::wvalue x;
        x = crow::json::wvalue::list();
        for (size_t i = 0; i < items.size(); ++i) {
            crow::json::wvalue nodeJson;
            nodeJson["id"] = items[i]->id;
            nodeJson["value"] = items[i]->value;
            x[i] = std::move(nodeJson);
        }
        return x;
    }
};

struct Queue {
    std::deque<Node*> items;

    void add(int val) {
        items.push_back(new Node(val));
    }

    void remove() {
        if (!items.empty()) {
            delete items.front();
            items.pop_front();
        }
    }

    void removeValue(int val) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((*it)->value == val) {
                delete* it;
                items.erase(it);
                return;
            }
        }
    }

    void clear() {
        for (auto n : items) delete n;
        items.clear();
    }

    crow::json::wvalue toJson() {
        crow::json::wvalue x;
        x = crow::json::wvalue::list();
        int i = 0;
        for (auto n : items) {
            crow::json::wvalue nodeJson;
            nodeJson["id"] = n->id;
            nodeJson["value"] = n->value;
            x[i++] = std::move(nodeJson);
        }
        return x;
    }
};

struct LinkedList {
    Node* head = nullptr;

    void add(int val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next) current = current->next;
            current->next = newNode;
        }
    }

    void remove() {
        if (!head) return;
        if (!head->next) {
            delete head;
            head = nullptr;
            return;
        }
        Node* current = head;
        while (current->next->next) current = current->next;
        delete current->next;
        current->next = nullptr;
    }

    void removeValue(int val) {
        if (!head) return;
        if (head->value == val) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node* current = head;
        while (current->next) {
            if (current->next->value == val) {
                Node* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return;
            }
            current = current->next;
        }
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    crow::json::wvalue toJson() {
        crow::json::wvalue x;
        x = crow::json::wvalue::list();
        Node* current = head;
        int i = 0;
        while (current) {
            crow::json::wvalue nodeJson;
            nodeJson["id"] = current->id;
            nodeJson["value"] = current->value;
            x[i++] = std::move(nodeJson);
            current = current->next;
        }
        return x;
    }
};

struct DoublyLinkedList {
    Node* head = nullptr;
    Node* tail = nullptr;

    void add(int val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void remove() {
        if (!tail) return;
        if (head == tail) {
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        else {
            Node* temp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete temp;
        }
    }

    void removeValue(int val) {
        Node* current = head;
        while (current) {
            if (current->value == val) {
                if (current->prev) current->prev->next = current->next;
                else head = current->next;

                if (current->next) current->next->prev = current->prev;
                else tail = current->prev;

                delete current;
                return;
            }
            current = current->next;
        }
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    crow::json::wvalue toJson() {
        crow::json::wvalue x;
        x = crow::json::wvalue::list();
        Node* current = head;
        int i = 0;
        while (current) {
            crow::json::wvalue nodeJson;
            nodeJson["id"] = current->id;
            nodeJson["value"] = current->value;
            x[i++] = std::move(nodeJson);
            current = current->next;
        }
        return x;
    }
};

struct MultiLinkedList {
    int maxLevel = 3;
    Node* head;

    MultiLinkedList() {
        head = new Node(-2147483648);
        head->forward.resize(maxLevel, nullptr);
    }

    void add(int val) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* current = head;

        for (int i = maxLevel - 1; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < val) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        int lvl = 1;
        while ((rand() % 2) == 0 && lvl < maxLevel) lvl++;

        Node* newNode = new Node(val);
        newNode->forward.resize(lvl, nullptr);
        newNode->maxLvl = lvl;

        for (int i = 0; i < lvl; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    void remove(int val) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* current = head;

        for (int i = maxLevel - 1; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->value < val) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        Node* target = current->forward[0];
        if (target && target->value == val) {
            for (int i = 0; i < maxLevel; i++) {
                if (i >= update.size() || update[i]->forward[i] != target) break;
                update[i]->forward[i] = target->forward[i];
            }
            delete target;
        }
    }

    void clear() {
        Node* current = head->forward[0];
        while (current) {
            Node* temp = current;
            current = current->forward[0];
            delete temp;
        }

        std::fill(head->forward.begin(), head->forward.end(), nullptr);
    }

    crow::json::wvalue toJson() {
        crow::json::wvalue x;
        x = crow::json::wvalue::list();

        Node* current = head->forward[0];
        int i = 0;
        while (current) {
            crow::json::wvalue nodeJson;
            nodeJson["id"] = current->id;
            nodeJson["value"] = current->value;
            nodeJson["maxLvl"] = current->maxLvl;

            crow::json::wvalue forwardIds;
            forwardIds = crow::json::wvalue::list();
            for (size_t k = 0; k < current->forward.size(); k++) {
                if (current->forward[k]) {
                    forwardIds[k] = current->forward[k]->id;
                }
                else {
                    forwardIds[k] = nullptr;
                }
            }
            nodeJson["forward"] = std::move(forwardIds);

            x[i++] = std::move(nodeJson);
            current = current->forward[0];
        }
        return x;
    }
};

struct BST {
    Node* root = nullptr;

    Node* insert(Node* node, int val) {
        if (!node) return new Node(val);
        if (val < node->value) node->left = insert(node->left, val);
        else if (val > node->value) node->right = insert(node->right, val);
        return node;
    }

    void add(int val) {
        root = insert(root, val);
    }

    Node* findMin(Node* node) {
        while (node->left) node = node->left;
        return node;
    }

    Node* removeNode(Node* node, int val) {
        if (!node) return nullptr;
        if (val < node->value) node->left = removeNode(node->left, val);
        else if (val > node->value) node->right = removeNode(node->right, val);
        else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = findMin(node->right);
            node->value = temp->value;
            node->right = removeNode(node->right, temp->value);
        }
        return node;
    }

    void remove(int val) {
        root = removeNode(root, val);
    }

    void clearNode(Node* node) {
        if (!node) return;
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }

    void clear() {
        clearNode(root);
        root = nullptr;
    }

    crow::json::wvalue nodeToJson(Node* node) {
        if (!node) return nullptr;
        crow::json::wvalue j;
        j["id"] = node->id;
        j["value"] = node->value;
        j["left"] = nodeToJson(node->left);
        j["right"] = nodeToJson(node->right);
        return j;
    }

    crow::json::wvalue toJson() {
        if (!root) return nullptr;
        return nodeToJson(root);
    }
};

Stack stackDS;
Queue queueDS;
LinkedList linkedListDS;
DoublyLinkedList doublyLinkedListDS;
MultiLinkedList multiLinkedListDS;
BST bstDS;

int main() {
    crow::SimpleApp app;

    auto cors = [](crow::response& res) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        };

    CROW_ROUTE(app, "/").methods("GET"_method)([&](const crow::request&, crow::response& res) {
        cors(res);
        res.set_static_file_info("templates/index.html");
        res.end();
    });

    CROW_ROUTE(app, "/api/<path>").methods("OPTIONS"_method)([&](const crow::request& req, crow::response& res, std::string path) {
        cors(res);
        res.end();
    });

    CROW_ROUTE(app, "/api/<string>/<string>/<path>").methods("OPTIONS"_method)([&](const crow::request& req, crow::response& res, std::string a, std::string b, std::string c) {
        cors(res);
        res.end();
    });

    
    CROW_ROUTE(app, "/api/<string>/add").methods("POST"_method)([&](const crow::request& req, crow::response& res, std::string type) {
        auto x = crow::json::load(req.body);
        if (!x) { res.code = 400; cors(res); res.end(); return; }

        int val = x["value"].i();

        if (type == "stack") stackDS.add(val);
        else if (type == "queue") queueDS.add(val);
        else if (type == "linkedlist") linkedListDS.add(val);
        else if (type == "doublylinkedlist") doublyLinkedListDS.add(val);
        else if (type == "multilinkedlist") multiLinkedListDS.add(val);
        else if (type == "bst") bstDS.add(val);

        cors(res);
        res.end("ok");
    });

    CROW_ROUTE(app, "/api/<string>/remove").methods("POST"_method)([&](const crow::request& req, crow::response& res, std::string type) {
        auto x = crow::json::load(req.body);
        bool hasValue = x && x.has("value");

        if (hasValue) {
            int val = x["value"].i();
            if (type == "stack") stackDS.removeValue(val);
            else if (type == "queue") queueDS.removeValue(val);
            else if (type == "linkedlist") linkedListDS.removeValue(val);
            else if (type == "doublylinkedlist") doublyLinkedListDS.removeValue(val);
            else if (type == "multilinkedlist") multiLinkedListDS.remove(val);
            else if (type == "bst") bstDS.remove(val);
        } else {
            if (type == "stack") stackDS.remove();
            else if (type == "queue") queueDS.remove();
            else if (type == "linkedlist") linkedListDS.remove();
            else if (type == "doublylinkedlist") doublyLinkedListDS.remove();
        }

        cors(res);
        res.end("ok");
    });

    CROW_ROUTE(app, "/api/<string>/clear").methods("POST"_method)([&](const crow::request& req, crow::response& res, std::string type) {
        if (type == "stack") stackDS.clear();
        else if (type == "queue") queueDS.clear();
        else if (type == "linkedlist") linkedListDS.clear();
        else if (type == "doublylinkedlist") doublyLinkedListDS.clear();
        else if (type == "multilinkedlist") multiLinkedListDS.clear();
        else if (type == "bst") bstDS.clear();

        cors(res);
        res.end("ok");
    });

    CROW_ROUTE(app, "/api/<string>/data").methods("GET"_method) ([&](const crow::request& req, crow::response& res, std::string type) {
        crow::json::wvalue json;

        if (type == "stack") json = stackDS.toJson();
        else if (type == "queue") json = queueDS.toJson();
        else if (type == "linkedlist") json = linkedListDS.toJson();
        else if (type == "doublylinkedlist") json = doublyLinkedListDS.toJson();
        else if (type == "multilinkedlist") json = multiLinkedListDS.toJson();
        else if (type == "bst") json = bstDS.toJson();

        cors(res);
        res.write(json.dump());
        res.end();
    });

    app.port(18080).multithreaded().run();
}