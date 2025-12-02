#include "crow.h"
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>
#include <iomanip>

std::string buatID() {
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

    Node(int val) : value(val), id(buatID()) {}
};

struct Stack {
    Node* atas = nullptr;

    void tambah(int val) {
        Node* nodeBaru = new Node(val);
        nodeBaru->next = atas;
        atas = nodeBaru;
    }

    void hapus() {
        if (!atas) return;
        Node* sementara = atas;
        atas = atas->next;
        delete sementara;
    }

    void hapusValue(int val) {
        if (!atas) return;

        if (atas->value == val) {
            hapus();
            return;
        }

        Node* nodeSekarang = atas;
        while (nodeSekarang->next) {
            if (nodeSekarang->next->value == val) {
                Node* sementara = nodeSekarang->next;
                nodeSekarang->next = nodeSekarang->next->next;
                delete sementara;
                return;
            }
            nodeSekarang = nodeSekarang->next;
        }
    }

    void clear() {
        while (atas) hapus();
    }

    crow::json::wvalue toJson() {
        std::vector<Node*> tempNodes;
        Node* current = atas;
        while (current) {
            tempNodes.push_back(current);
            current = current->next;
        }

        std::reverse(tempNodes.begin(), tempNodes.end());

        crow::json::wvalue x;
        x = crow::json::wvalue::list();
        for (size_t i = 0; i < tempNodes.size(); ++i) {
            crow::json::wvalue nodeJson;
            nodeJson["id"] = tempNodes[i]->id;
            nodeJson["value"] = tempNodes[i]->value;
            x[i] = std::move(nodeJson);
        }
        return x;
    }
};

struct Queue {
    Node* palak = nullptr;
    Node* ekor = nullptr;

    void tambah(int val) {
        Node* nodeBaru = new Node(val);
        if (!ekor) {
            palak = ekor = nodeBaru;
        }
        else {
            ekor->next = nodeBaru;
            ekor = nodeBaru;
        }
    }

    void hapus() {
        if (!palak) return;

        Node* sementara = palak;
        palak = palak->next;

        if (!palak) {
            ekor = nullptr;
        }

        delete sementara;
    }

    void hapusValue(int val) {
        if (!palak) return;

        if (palak->value == val) {
            hapus();
            return;
        }

        Node* nodeSekarang = palak;
        while (nodeSekarang->next) {
            if (nodeSekarang->next->value == val) {
                Node* sementara = nodeSekarang->next;
                nodeSekarang->next = sementara->next;

                if (sementara == ekor) {
                    ekor = nodeSekarang;
                }

                delete sementara;
                return;
            }
            nodeSekarang = nodeSekarang->next;
        }
    }

    void clear() {
        while (palak) hapus();
    }

    crow::json::wvalue toJson() {
        crow::json::wvalue x;
        x = crow::json::wvalue::list();

        Node* current = palak;
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

struct LinkedList {
    Node* palak = nullptr;

    void add(int val) {
        Node* nodeBaru = new Node(val);
        if (!palak) {
            palak = nodeBaru;
        } else {
            Node* nodeSekarang = palak;
            while (nodeSekarang->next) nodeSekarang = nodeSekarang->next;
            nodeSekarang->next = nodeBaru;
        }
    }

    void hapus() {
        if (!palak) return;
        if (!palak->next) {
            delete palak;
            palak = nullptr;
            return;
        }
        Node* nodeSekarang = palak;
        while (nodeSekarang->next->next) nodeSekarang = nodeSekarang->next;
        delete nodeSekarang->next;
        nodeSekarang->next = nullptr;
    }

    void hapusValue(int val) {
        if (!palak) return;
        
        if (palak->value == val) {
            Node* sementara = palak;
            palak = palak->next;
            delete sementara;
            return;
        }

        Node* nodeSekarang = palak;
        while (nodeSekarang->next) {
            if (nodeSekarang->next->value == val) {
                Node* sementara = nodeSekarang->next;
                nodeSekarang->next = nodeSekarang->next->next;
                delete sementara;
                
                return;
            }

            nodeSekarang = nodeSekarang->next;
        }
    }

    void clear() {
        while (palak) {
            Node* temp = palak;
            palak = palak->next;
            delete temp;
        }
    }

    crow::json::wvalue toJson() {
        crow::json::wvalue x;
        x = crow::json::wvalue::list();
        Node* current = palak;
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
    Node* palak = nullptr;
    Node* ekor = nullptr;

    void add(int val) {
        Node* nodeBaru = new Node(val);
        if (!palak) {
            palak = nodeBaru;
            ekor = nodeBaru;
        }
        else {
            ekor->next = nodeBaru;
            nodeBaru->prev = ekor;
            ekor = nodeBaru;
        }
    }

    void hapus() {
        if (!ekor) return;
        if (palak == ekor) {
            delete palak;
            palak = nullptr;
            ekor = nullptr;
        } else {
            Node* sementara = ekor;
            ekor = ekor->prev;
            ekor->next = nullptr;
            delete sementara;
        }
    }

    void hapusValue(int val) {
        Node* nodeSekarang = palak;
        while (nodeSekarang) {
            if (nodeSekarang->value == val) {
                if (nodeSekarang->prev) nodeSekarang->prev->next = nodeSekarang->next;
                else palak = nodeSekarang->next;

                if (nodeSekarang->next) nodeSekarang->next->prev = nodeSekarang->prev;
                else ekor = nodeSekarang->prev;

                delete nodeSekarang;
                return;
            }

            nodeSekarang = nodeSekarang->next;
        }
    }

    void clear() {
        while (palak) {
            Node* temp = palak;
            palak = palak->next;
            delete temp;
        }

        ekor = nullptr;
    }

    crow::json::wvalue toJson() {
        crow::json::wvalue x;
        x = crow::json::wvalue::list();
        Node* current = palak;
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
    int maximumJumlahNode = 3;
    Node* palak;

    MultiLinkedList() {
        palak = new Node(-2147483648);
        palak->forward.resize(maximumJumlahNode, nullptr);
    }

    void tambah(int val) {
        std::vector<Node*> update(maximumJumlahNode, nullptr);
        Node* nodeSekarang = palak;

        for (int i = maximumJumlahNode - 1; i >= 0; i--) {
            while (nodeSekarang->forward[i] && nodeSekarang->forward[i]->value < val) {
                nodeSekarang = nodeSekarang->forward[i];
            }
            update[i] = nodeSekarang;
        }

        int lvl = 1;
        while ((rand() % 2) == 0 && lvl < maximumJumlahNode) lvl++;

        Node* nodeBaru = new Node(val);
        nodeBaru->forward.resize(lvl, nullptr);
        nodeBaru->maxLvl = lvl;

        for (int i = 0; i < lvl; i++) {
            nodeBaru->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = nodeBaru;
        }
    }

    void hapus(int val) {
        std::vector<Node*> update(maximumJumlahNode, nullptr);
        Node* nodeSekarang = palak;

        for (int i = maximumJumlahNode - 1; i >= 0; i--) {
            while (nodeSekarang->forward[i] && nodeSekarang->forward[i]->value < val) {
                nodeSekarang = nodeSekarang->forward[i];
            }

            update[i] = nodeSekarang;
        }

        Node* target = nodeSekarang->forward[0];
        if (target && target->value == val) {
            for (int i = 0; i < maximumJumlahNode; i++) {
                if (i >= update.size() || update[i]->forward[i] != target) break;
                update[i]->forward[i] = target->forward[i];
            }

            delete target;
        }
    }

    void clear() {
        Node* current = palak->forward[0];
        while (current) {
            Node* temp = current;
            current = current->forward[0];
            delete temp;
        }

        std::fill(palak->forward.begin(), palak->forward.end(), nullptr);
    }

    crow::json::wvalue toJson() {
        crow::json::wvalue x;
        x = crow::json::wvalue::list();

        Node* current = palak->forward[0];
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
    Node* akar = nullptr;

    Node* masukin(Node* node, int val) {
        if (!node) return new Node(val);
        if (val < node->value) node->left = masukin(node->left, val);
        else if (val > node->value) node->right = masukin(node->right, val);
        return node;
    }

    void tambah(int val) {
        akar = masukin(akar, val);
    }

    Node* cariTerkecil(Node* node) {
        while (node->left) node = node->left;
        return node;
    }

    Node* hapusNode(Node* node, int val) {
        if (!node) return nullptr;
        if (val < node->value) node->left = hapusNode(node->left, val);
        else if (val > node->value) node->right = hapusNode(node->right, val);
        else {
            if (!node->left) {
                Node* sementara = node->right;
                delete node;
                return sementara;
            }
            else if (!node->right) {
                Node* sementara = node->left;
                delete node;
                return sementara;
            }
            Node* sementara = cariTerkecil(node->right);
            node->value = sementara->value;
            node->right = hapusNode(node->right, sementara->value);
        }

        return node;
    }

    void hapus(int val) {
        akar = hapusNode(akar, val);
    }

    void clearNode(Node* node) {
        if (!node) return;
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }

    void clear() {
        clearNode(akar);
        akar = nullptr;
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
        if (!akar) return nullptr;
        return nodeToJson(akar);
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

        if (type == "stack") stackDS.tambah(val);
        else if (type == "queue") queueDS.tambah(val);
        else if (type == "linkedlist") linkedListDS.add(val);
        else if (type == "doublylinkedlist") doublyLinkedListDS.add(val);
        else if (type == "multilinkedlist") multiLinkedListDS.tambah(val);
        else if (type == "bst") bstDS.tambah(val);

        cors(res);
        res.end("ok");
    });

    CROW_ROUTE(app, "/api/<string>/remove").methods("POST"_method)([&](const crow::request& req, crow::response& res, std::string type) {
        auto x = crow::json::load(req.body);
        bool hasValue = x && x.has("value");

        if (hasValue) {
            int val = x["value"].i();
            if (type == "stack") stackDS.hapusValue(val);
            else if (type == "queue") queueDS.hapusValue(val);
            else if (type == "linkedlist") linkedListDS.hapusValue(val);
            else if (type == "doublylinkedlist") doublyLinkedListDS.hapusValue(val);
            else if (type == "multilinkedlist") multiLinkedListDS.hapus(val);
            else if (type == "bst") bstDS.hapus(val);
        } else {
            if (type == "stack") stackDS.hapus();
            else if (type == "queue") queueDS.hapus();
            else if (type == "linkedlist") linkedListDS.hapus();
            else if (type == "doublylinkedlist") doublyLinkedListDS.hapus();
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