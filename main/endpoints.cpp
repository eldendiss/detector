#include "endpoints.h"
#include "esp_log.h"
#include "string.h"
#include "movement.h"

static const char *TAG = "endpoints";

static int X=0;
static int Y=0;
static int pos=1;

static esp_err_t XY_handler(httpd_req_t *req){
    char rq[30];
    esp_err_t ret = httpd_req_recv(req,rq,30);
    /*if(ret != ESP_OK){
        ESP_LOGE(TAG,"XY uri error %s",esp_err_to_name(ret));
        return ESP_FAIL;
    }*/
    int r = sscanf(rq, "%d %d",&X, &Y);
    if(r!=2){
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }
    if(X == 0 && Y == 0){
        ESP_LOGI(TAG,"Received home pos: X %d-Y %d. Homing...",X,Y);
        home(NULL);
        ESP_LOGI(TAG, "Homing done");
    } else {
        ESP_LOGI(TAG,"Received coordinates: X %d-Y %d. Moving...",X,Y);
        moveXY(X,Y);
        ESP_LOGI(TAG, "Move done");
    }
    char resp[30];
    getXYZ(&X,&Y,&pos);
    sprintf(resp,"%d %d",X,Y);
    httpd_resp_send(req,resp,strlen(resp));

    return ESP_OK;
}

static esp_err_t Z_handler(httpd_req_t *req){
    char rq[30];
    esp_err_t ret = httpd_req_recv(req,rq,30);
    /*if(ret != ESP_OK){
        ESP_LOGE(TAG,"Z uri error %s",esp_err_to_name(ret));
        return ESP_FAIL;
    }*/

    int r=sscanf(rq, "%d", &pos); //0 down, 1 up
    if(r!=1){
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG,"Received Z position: %d mm. Moving...",pos);
    moveZ(pos);
    ESP_LOGI(TAG, "Move done");
    char resp[30];
    getXYZ(&X,&Y,&pos);
    sprintf(resp,"%d",pos);
    httpd_resp_send(req,resp,strlen(resp));
    return ESP_OK;
}

static esp_err_t status_handler(httpd_req_t *req){
    char resp[200];
    getXYZ(&X,&Y,&pos);
    sprintf(resp,"pos(X Y Z)=%d %d %d\nstatus=OK\nversion=0.0.1",X,Y,pos);
    httpd_resp_send(req,resp,strlen(resp));
    return ESP_OK;
}

static const httpd_uri_t XY = {
    .uri = "/XY",
    .method = HTTP_POST,
    .handler = XY_handler
};

static const httpd_uri_t Z = {
    .uri = "/Z",
    .method = HTTP_POST,
    .handler = Z_handler
};

static const httpd_uri_t status = {
    .uri = "/status",
    .method = HTTP_GET,
    .handler = status_handler
};  

httpd_handle_t startServer() {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 8080;
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &XY);
        httpd_register_uri_handler(server, &Z);
        httpd_register_uri_handler(server, &status);
        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}