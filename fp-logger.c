#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPERANDS 10
#define MAX_ENTRIES 100

typedef struct {
  double minRes;
  double maxRes;
  double minOperands[MAX_OPERANDS];
  double maxOperands[MAX_OPERANDS];
  unsigned executions;
} ValueInfo;

typedef struct {
  double minErr;
  double maxErr;
} ErrorInfo;

typedef struct {
  double grad;
} GradInfo;

typedef struct {
  char id[50];
  ValueInfo valueInfo;
} ValueEntry;

typedef struct {
  char id[50];
  ErrorInfo errorInfo;
} ErrorEntry;

typedef struct {
  char id[50];
  GradInfo gradInfo;
} GradEntry;

static ValueEntry valueEntries[MAX_ENTRIES];
static ErrorEntry errorEntries[MAX_ENTRIES];
static GradEntry gradEntries[MAX_ENTRIES];
static int valueCount = 0, errorCount = 0, gradCount = 0;

void updateValue(const char *id, double res, unsigned numOperands,
                 const double *operands) {
  ValueInfo *info = NULL;
  for (int i = 0; i < valueCount; ++i) {
    if (strcmp(valueEntries[i].id, id) == 0) {
      info = &valueEntries[i].valueInfo;
      break;
    }
  }
  if (info == NULL) {
    assert(valueCount < MAX_ENTRIES);
    strcpy(valueEntries[valueCount].id, id);
    info = &valueEntries[valueCount].valueInfo;
    info->minRes = DBL_MAX;
    info->maxRes = -DBL_MAX;
    for (unsigned i = 0; i < MAX_OPERANDS; i++) {
      info->minOperands[i] = DBL_MAX;
      info->maxOperands[i] = -DBL_MAX;
    }
    valueCount++;
  }

  info->minRes = fmin(info->minRes, res);
  info->maxRes = fmax(info->maxRes, res);
  for (unsigned i = 0; i < numOperands; ++i) {
    info->minOperands[i] = fmin(info->minOperands[i], operands[i]);
    info->maxOperands[i] = fmax(info->maxOperands[i], operands[i]);
  }
  ++info->executions;
}

void updateError(const char *id, double err) {
  ErrorInfo *info = NULL;
  for (int i = 0; i < errorCount; ++i) {
    if (strcmp(errorEntries[i].id, id) == 0) {
      info = &errorEntries[i].errorInfo;
      break;
    }
  }
  if (info == NULL) {
    assert(errorCount < MAX_ENTRIES);
    strcpy(errorEntries[errorCount].id, id);
    info = &errorEntries[errorCount].errorInfo;
    info->minErr = DBL_MAX;
    info->maxErr = -DBL_MAX;
    errorCount++;
  }

  info->minErr = fmin(info->minErr, err);
  info->maxErr = fmax(info->maxErr, err);
}

void updateGrad(const char *id, double grad) {
  GradInfo *info = NULL;
  for (int i = 0; i < gradCount; ++i) {
    if (strcmp(gradEntries[i].id, id) == 0) {
      info = &gradEntries[i].gradInfo;
      break;
    }
  }
  if (info == NULL) {
    assert(gradCount < MAX_ENTRIES);
    strcpy(gradEntries[gradCount].id, id);
    info = &gradEntries[gradCount].gradInfo;
    gradCount++;
  }

  info->grad = grad;
}

void printLogger() {
  for (int i = 0; i < valueCount; ++i) {
    ValueInfo *info = &valueEntries[i].valueInfo;
    printf("Value:%s\n", valueEntries[i].id);
    printf("\tMinRes = %e\n", info->minRes);
    printf("\tMaxRes = %e\n", info->maxRes);
    printf("\tExecutions = %u\n", info->executions);
    for (int j = 0; j < MAX_OPERANDS; ++j) {
      if (info->minOperands[j] != DBL_MAX) {
        printf("\tOperand[%d] = [%e, %e]\n", j, info->minOperands[j],
               info->maxOperands[j]);
      }
    }
  }

  for (int i = 0; i < errorCount; ++i) {
    ErrorInfo *info = &errorEntries[i].errorInfo;
    printf("Error:%s\n", errorEntries[i].id);
    printf("\tMinErr = %e\n", info->minErr);
    printf("\tMaxErr = %e\n", info->maxErr);
  }

  for (int i = 0; i < gradCount; ++i) {
    GradInfo *info = &gradEntries[i].gradInfo;
    printf("Grad:%s\n", gradEntries[i].id);
    printf("\tGrad = %e\n", info->grad);
  }
}
