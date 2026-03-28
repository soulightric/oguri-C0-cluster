#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <vector>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

class ProxmoxAPI {
private:
    std::string base_url;
    std::string ticket;
    std::string csrf_token;
    bool verify_ssl; 

public:
    ProxmoxAPI(const std::string& host, bool verify = false) 
        : base_url("https://" + host + ":8006/api2/json/"), verify_ssl(verify) {}

    bool login(const std::string& username, const std::string& password) {
        CURL* curl = curl_easy_init();
        if (!curl) return false;

        std::string url = base_url + "access/ticket";
        std::string post_data = "username=" + username + "&password=" + password;
        std::string response;

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verify_ssl ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verify_ssl ? 2L : 0L);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (res != CURLE_OK) {
            std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
            return false;
        }

        try {
            json j = json::parse(response);
            if (j.contains("data")) {
                auto data = j["data"];
                ticket = data["ticket"];
                csrf_token = data["CSRFPreventionToken"];
                std::cout << "Login berhasil!" << std::endl;
                return true;
            }
        } catch (...) {
            std::cerr << "Gagal parse JSON response" << std::endl;
        }
        return false;
    }

    std::string request(const std::string& endpoint, const std::string& method = "GET", 
                       const json& body = nullptr) {
        CURL* curl = curl_easy_init();
        std::string response;
        std::string full_url = base_url + endpoint;

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Cookie: PVEAuthCookie=" + ticket).c_str());
        headers = curl_slist_append(headers, ("CSRFPreventionToken: " + csrf_token).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verify_ssl ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verify_ssl ? 2L : 0L);

        if (method == "POST" && body != nullptr) {
            std::string json_str = body.dump();
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        } else if (method == "PUT") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        } else if (method == "DELETE") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        }

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        if (res != CURLE_OK) {
            std::cerr << "Request error: " << curl_easy_strerror(res) << std::endl;
            return "";
        }
        return response;
    }

    bool createVM(int node_index = 0, int vmid = 100, const std::string& node = "pve") {
        json config = {
            {"vmid", vmid},
            {"name", "cpp-test-vm"},
            {"memory", 2048},
            {"cores", 2},
            {"sockets", 1},
            {"net0", "virtio,bridge=vmbr0"},
            {"ostype", "l26"},
            {"scsi0", "local-lvm:8"}, 
            {"boot", "order=scsi0"},
            {"agent", "1"}
        };

        std::string endpoint = "nodes/" + node + "/qemu";
        std::string resp = request(endpoint, "POST", config);
        
        std::cout << "Response create VM: " << resp << std::endl;
        return !resp.empty();
    }

    json getVMConfig(const std::string& node, int vmid) {
        std::string endpoint = "nodes/" + node + "/qemu/" + std::to_string(vmid) + "/config";
        std::string resp = request(endpoint, "GET");
        
        try {
            json j = json::parse(resp);
            if (j.contains("data")) return j["data"];
        } catch (...) {}
        return json();
    }
};

int main() {
    std::string host = "192.168.1.100";
    ProxmoxAPI api(host, false);

    std::string username = "root@pam";
    std::string password = "password_kamu";

    if (api.login(username, password)) {
        std::cout << "=== Proxmox Configuration via C++ ===" << std::endl;

        json config = api.getVMConfig("pve", 100);
        if (!config.empty()) {
            std::cout << "Konfigurasi VM 100:" << std::endl;
            std::cout << config.dump(4) << std::endl;
        }
    }

    return 0;
}
