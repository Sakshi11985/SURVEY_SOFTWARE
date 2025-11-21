#ifndef SURVEY_H
#define SURVEY_H

typedef struct BSTNode {
    char option[50];
    int count;
    struct BSTNode *left, *right;
} BSTNode;

typedef struct Question {
    char text[200];
    char options[5][50];
    int numOptions;
    BSTNode *responses;
    int totalResponses;
    struct Question *next;
} Question;

typedef struct Survey {
    char title[100];
    Question *questions;
    int conducted;
    struct Survey *next;
} Survey;

typedef Survey SurveyNode;

BSTNode* createBSTNode(char *option);
BSTNode* insertBST(BSTNode *root, char *option);
BSTNode* searchBST(BSTNode *root, const char *key);
int totalResponsesBST(BSTNode *root);

Question* newQuestion(char *text, int numOptions);
void addSurveyName(SurveyNode **head);
void addQuestionToSurvey(SurveyNode *s);
void viewSurveyDetails(SurveyNode *head);
void conductSurvey(SurveyNode *head);
void publishResults(SurveyNode *head);

SurveyNode* createSurveyNode(char *title);
SurveyNode* selectSurveyWithQuestions(SurveyNode *head);
SurveyNode* selectSurveyConducted(SurveyNode *head);
SurveyNode* selectAnySurvey(SurveyNode *head);

void deleteQuestion(SurveyNode *head);
void deleteSurvey(SurveyNode **head);

#endif
