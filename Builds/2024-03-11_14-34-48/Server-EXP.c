#include <stdio.h>
#include <microhttpd.h>

#define CONFIG_FILE "Server-EXP.jcl"

struct ServerConfig {
    int port;
    int debugMode;
};

// Function to parse the configuration file
struct ServerConfig parseConfigFile(const char *filename) {
    struct ServerConfig config;
    // Open the configuration file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening config file");
        exit(EXIT_FAILURE);
    }

    // Read and parse the configuration
    fscanf(file, "// Server Configuration\n");
    fscanf(file, "\"port\": %d,\n", &config.port);
    fscanf(file, "\"debugMode\": %d", &config.debugMode);

    // Close the file
    fclose(file);

    return config;
}

// HTTP request handler function
int requestHandler(void *cls, struct MHD_Connection *connection, const char *url,
                   const char *method, const char *version, const char *upload_data,
                   size_t *upload_data_size, void **con_cls) {
    const char *page = "<html><body>Hello, world! :3</body></html>";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(page),
                                                                    (void *)page,
                                                                    MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

int main() {
    // Parse the configuration file
    struct ServerConfig config = parseConfigFile(CONFIG_FILE);

    // Start the HTTP server
    struct MHD_Daemon *daemon = MHD_start_daemon(MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD,
                                                 config.port, NULL, NULL,
                                                 &requestHandler, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        fprintf(stderr, "Error starting server on port %d\n", config.port);
        return 1;
    }

    printf("Server started on port %d\n", config.port);

    // Keep the server running
    getchar();

    // Stop the server
    MHD_stop_daemon(daemon);

    return 0;
}
