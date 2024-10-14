#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <algorithm> // For remove_if
#include <cctype>    // For isspace
#include <sstream>
#include <cstdio>   // For running shell commands
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace beast = boost::beast;  // from <boost/beast.hpp>
namespace http = beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;     // from <boost/asio.hpp>
using tcp = net::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

void load_env_file(const std::string& file_path) {
    std::ifstream env_file(file_path);
    if (!env_file.is_open()) {
        std::cerr << "Error: Failed to open .env file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(env_file, line)) {
        std::istringstream line_stream(line);
        std::string key, value;
        if (std::getline(line_stream, key, '=') && std::getline(line_stream, value)) {
            setenv(key.c_str(), value.c_str(), 1); // Set the environment variable
        }
    }
}

// This is a placeholder callback for handling responses
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

// Helper function to escape quotes and special characters for JSON
std::string escape_quotes(const std::string& input) {
    std::ostringstream escaped;
    for (char c : input) {
        switch (c) {
            case '"':  escaped << "\\\""; break; // Escape double quotes
            case '\\': escaped << "\\\\"; break; // Escape backslashes
            case '\n': escaped << "\\n"; break;  // Escape newlines
            case '\r': escaped << "\\r"; break;  // Escape carriage returns
            case '\t': escaped << "\\t"; break;  // Escape tabs
            default:   escaped << c;             // Default case, keep character as is
        }
    }
    return escaped.str();
}

std::string call_openai_api(const std::string& prompt) {
    const char* api_key = std::getenv("API_KEY");
    if (!api_key) {
        std::cerr << "Error: API_KEY environment variable is not set" << std::endl;
        return "{\"error\": \"API_KEY not set\"}";
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error: Failed to initialize CURL" << std::endl;
        return "{\"error\": \"Failed to initialize CURL\"}";
    }

    CURLcode res;
    std::string response_string;

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // Add the OpenAI API key in the header
    std::string api_header = std::string("Authorization: Bearer ") + std::string(api_key);
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, api_header.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Escape the prompt to ensure it is safe for JSON
    std::string escaped_prompt = escape_quotes(prompt);

    // Construct the JSON request payload for chat completion
    std::string request_body = "{\"model\":\"gpt-4\", \"messages\":[{\"role\":\"user\", \"content\":\"" + escaped_prompt + "\"}], \"temperature\":0.7}";
    std::cout << "Sending JSON request body: " << request_body << std::endl;  // Debugging output

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    // Perform the request
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
        response_string = "{\"error\": \"CURL request failed\"}";
    } else {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        std::cout << "HTTP Status Code: " << http_code << std::endl;
        
        if (http_code >= 400) {
            std::cerr << "Error: Received HTTP error code: " << http_code << std::endl;
            std::cerr << "Response from OpenAI: " << response_string << std::endl;
        } else {
            std::cout << "OpenAI Response: " << response_string << std::endl;
        }
    }

    // Clean up resources
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response_string;
}

// Helper function to generate random arrays
std::vector<int> generate_random_array(int size, int min_value, int max_value) {
    std::vector<int> arr(size);
    for (int& x : arr) {
        x = rand() % (max_value - min_value + 1) + min_value;
    }
    return arr;
}

// Convert array to JSON format
std::string array_to_json(const std::vector<int>& array) {
    json json_array = array;
    return json_array.dump();
}

// Convert test case array to a properly escaped JSON string
std::string escape_json_string(const std::vector<int>& test_case) {
    return json(test_case).dump();  // Serialize test case to JSON string
}

std::string run_user_code(const std::string& code, const std::vector<std::vector<int>>& test_cases) {
    // Debugging: Print the received code
    std::cout << "User's code received (raw):\n" << code << std::endl;

    // Write the user's code to a file (example: user_code.py)
    std::ofstream code_file("user_code.py");
    if (!code_file) {
        return "Error: Unable to create user_code.py file";
    }
    code_file << code;
    code_file.close();

    std::string results;

    // Loop through each test case
    for (int i = 0; i < test_cases.size(); ++i) {
        // Serialize the test case array into a JSON string and write it to a file
        std::string input = json(test_cases[i]).dump();  // Serialize array to JSON
        
        std::ofstream input_file("input.json");
        if (!input_file) {
            return "Error: Unable to create input.json file";
        }
        input_file << input;
        input_file.close();

        // Debugging: Print that the input file was created
        std::cout << "Test case " << i + 1 << " input written to input.json: " << input << std::endl;

        // Run the Python script, passing the file name (input.json)
        std::string command = "python3 user_code.py input.json > output.txt";
        std::cout << "Running command: " << command << std::endl;
        int system_return = std::system(command.c_str());

        if (system_return != 0) {
            results += "Error: Failed to execute user code for test case " + std::to_string(i + 1) + "\n";
            continue;
        }

        // Read the output from the file and capture the last line (which contains the actual result)
        std::ifstream output_file("output.txt");
        if (!output_file) {
            results += "Error: Unable to read output.txt for test case " + std::to_string(i + 1) + "\n";
            continue;
        }

        std::string line;
        std::string last_output;
        while (std::getline(output_file, line)) {
            last_output = line;  // Store the last line of the output
        }
        output_file.close();

        // Collecting results for each test case
        results += "Test case " + std::to_string(i + 1) + ": " + last_output + "\n";
    }

    json response;
    response["evaluation"] = results;  // Results are a plain string

    // Debugging: Print the response before serialization
    std::cout << "Backend JSON object (before serialization): " << response.dump(4) << std::endl;

    // Return the serialized JSON object as a string
    std::string final_response = response.dump();  // Serialize the response

    // Debugging: Print the actual string being returned
    std::cout << "Final response string (before return): " << final_response << std::endl;

    return final_response; 
}

void handle_request(http::request<http::string_body> const& req, http::response<http::string_body>& res) {
    // Add CORS headers to allow cross-origin requests
    res.set(http::field::access_control_allow_origin, "*");
    res.set(http::field::access_control_allow_methods, "POST, GET, OPTIONS");
    res.set(http::field::access_control_allow_headers, "Content-Type");

    if (req.method() == http::verb::options) {
        res.result(http::status::no_content);
        res.prepare_payload();
        return;
    }

    if (req.method() == http::verb::post && req.target() == "/api/solve") {
        // Extract the prompt and user code from the request body
        std::string request_body = req.body();
        json request_json;
        try {
            request_json = json::parse(request_body);
        } catch (const json::parse_error& e) {
            std::cerr << "Error parsing JSON request body: " << e.what() << std::endl;
            res.result(http::status::bad_request);
            res.body() = "{\"error\": \"Invalid JSON format\"}";
            res.prepare_payload();
            return;
        }

        if (!request_json.contains("prompt") || !request_json.contains("code")) {
            std::cerr << "Error: 'prompt' or 'code' field missing in the request body" << std::endl;
            res.result(http::status::bad_request);
            res.body() = "{\"error\": \"Missing 'prompt' or 'code' field\"}";
            res.prepare_payload();
            return;
        }

        std::string prompt = request_json["prompt"];
        std::string user_code = request_json["code"];

        // Generate test cases (example: for an array-based problem)
        std::vector<int> test_case_1 = generate_random_array(10, 1, 100);  // Small array
        std::vector<int> test_case_2 = generate_random_array(100, 1, 1000);  // Large array
        std::vector<int> test_case_3 = {1, 1, 1, 1, 1, 1, 1, 1};  // Edge case (all elements the same)
        std::vector<std::vector<int>> test_cases = {test_case_1, test_case_2, test_case_3};

        // Run the user's code and evaluate it against the test cases
        std::string results = run_user_code(user_code, test_cases);

        // Return the evaluation results
        res.result(http::status::ok);
        res.set(http::field::server, "Boost.Beast");
        res.set(http::field::content_type, "application/json");
        // res.body() = "{\"evaluation\": \"" + results + "\"}";
        res.body() = results;
        res.prepare_payload();
    } else {
        res.result(http::status::not_found);
        res.set(http::field::content_type, "text/plain");
        res.body() = "Not Found";
        res.prepare_payload();
    }
}


// Function to handle a single session
void do_session(tcp::socket socket) {
    try {
        beast::flat_buffer buffer;

        // Read a request
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        // Prepare a response
        http::response<http::string_body> res;
        handle_request(req, res);

        // Write the response
        http::write(socket, res);

        // Close the socket
        socket.shutdown(tcp::socket::shutdown_send);
    } catch (std::exception const& e) {
        std::cerr << "Error in session: " << e.what() << std::endl;
    }
}

// Function to accept and handle incoming connections
void server(tcp::acceptor& acceptor, net::io_context& ioc) {
    for (;;) {
        try {
            // Accept a new connection
            tcp::socket socket(ioc);
            acceptor.accept(socket);

            // Handle the session in a new thread
            std::thread(&do_session, std::move(socket)).detach();
        } catch (std::exception& e) {
            std::cerr << "Error in server: " << e.what() << std::endl;
        }
    }
}

int main() {
    // Load environment variables from .env file
    load_env_file("/Users/williambach/Desktop/ProgramHelper/ProgramHelper/.env");

    try {
        net::io_context ioc;

        // Create an acceptor to listen for new connections
        tcp::acceptor acceptor(ioc, {tcp::v4(), 5000});

        std::cout << "Server running on port 5000" << std::endl;

        // Run the server loop
        server(acceptor, ioc);

    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}