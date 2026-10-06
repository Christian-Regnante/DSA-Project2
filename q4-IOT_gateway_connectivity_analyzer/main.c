#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gateway_network.h"

static void display_available_gateways(void)
{
    printf("--------------------------------------------------------\n");
    printf("Available Gateways in Network: ");
    for (int i = 0; i < NUM_GATEWAYS; i++)
    {
        printf("%c%s", gateway_index_to_name(i), (i < NUM_GATEWAYS - 1) ? ", " : "\n");
    }
    printf("--------------------------------------------------------\n");
}

int main(void)
{
    Graph network;
    graph_build_iot_network(&network);

    printf("========================================================\n");
    printf("      SMART AGRICULTURE: IoT GATEWAY ANALYZER           \n");
    printf("========================================================\n");
    display_available_gateways();

    char line_buffer[128];
    char input_gateway = '\0';
    int start_index = -1;

    /* Task 1: Gateway Selection & Robust Input Validation */
    while (1)
    {
        printf("\nEnter starting gateway: ");
        if (!fgets(line_buffer, sizeof(line_buffer), stdin))
        {
            printf("\nEnd of input encountered. Exiting...\n");
            return 0;
        }

        /* Trim leading whitespace */
        char *ptr = line_buffer;
        while (*ptr && isspace((unsigned char)*ptr))
        {
            ptr++;
        }

        /* Check for empty input */
        if (*ptr == '\0')
        {
            printf("Error: Input cannot be empty. Please enter a gateway (A-G).\n");
            continue;
        }

        /* Check for multiple characters (e.g., "ABC" or "Gate1") */
        char candidate = *ptr;
        ptr++;
        while (*ptr && isspace((unsigned char)*ptr))
        {
            ptr++;
        }

        if (*ptr != '\0')
        {
            printf("Error: Multiple characters detected. Please enter a single gateway character (A-G).\n");
            continue;
        }

        /* Validate against network gateways */
        start_index = gateway_name_to_index(candidate);
        if (start_index == -1)
        {
            printf("Error: Gateway '%c' does not exist in the network. Valid gateways are A through G.\n",
                   candidate);
            continue;
        }

        input_gateway = (char)toupper((unsigned char)candidate);
        break;
    }

    printf("Selected Gateway: %c (Validated successfully)\n", input_gateway);

    /* Tasks 2 & 3: BFS Connectivity and Communication Analysis */
    AnalysisResult result = analyze_gateway_connectivity(&network, start_index);
    print_analysis_report(&result);

    return 0;
}
