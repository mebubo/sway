#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <strings.h>
#include "sway/config.h"
#include "sway/commands.h"
#include "sway/input/input-manager.h"
#include "log.h"
#include "stringop.h"
#include "util.h"

struct cmd_results *input_cmd_calibration_matrix(int argc, char **argv) {
	struct cmd_results *error = NULL;
	if ((error = checkarg(argc, "calibration_matrix", EXPECTED_EQUAL_TO, 1))) {
		return error;
	}
	struct input_config *ic = config->handler_context.input_config;
	if (!ic) {
		return cmd_results_new(CMD_FAILURE, "No input device defined.");
	}

	list_t *split = split_string(argv[0], " ");
	if (split->length != 6) {
		return cmd_results_new(CMD_FAILURE, "calibration_matrix should be a space-separated list of length 6");
	}

	float parsed[6];
	for (int i = 0; i < split->length; ++i) {
		char *item = split->items[i];
		float x = parse_float(item);
		if (x != x) {
			return cmd_results_new(CMD_FAILURE, "calibration_matrix: unable to parse float: %s", item);
		}
		parsed[i] = x;
	}

	int size = sizeof(parsed);
	ic->calibration_matrix = malloc(size);
	memcpy(ic->calibration_matrix, parsed, size);

	return cmd_results_new(CMD_SUCCESS, NULL);
}
