# REPORT
The data structures used here are Linked list and BST.

## Linked List
The Linked List is used to store surveys and their associated questions.
Each survey node points to its questions, allowing easy sequential access.   

Functions using Linked lists only are:
- createSurveyNode() 
- addQuestion()  
- newQuestion()  
- addQuestionToSurvey()  
- viewSurveyDetails()   
- selectSurveyWithQuestions()  
- selectSurveyConducted()  
- selectAnySurvey()  

## BST
The BST stores the responses for each question. Each response option is a node in the tree, and the number of times each option is selected is stored in the node's count field.
This structure allows efficient counting and updating of responses.  

Functions using BST only are:
- createBSTNode()  
- insertBST()  
- searchBST()  
- totalResponsesBST()  

## Interaction of Linked Lists and BST:  
The Linked List in the Survey Management System is used to organize surveys and their questions sequentially, while the Binary Search Tree (BST) attached to each question stores and counts user responses efficiently.
During survey conduction, the program traverses the linked list of questions and inserts each response into the corresponding question’s BST.
When publishing results, it again traverses the linked list and accesses each BST to compute counts and percentages.
Thus, the Linked List manages the survey structure, and the BST handles response analytics — working together to form a complete survey analysis system.   

#### Functions using both data structures are:  
- conductSurvey()   
- publishResults()
  
## FLOWCHART
<img width="464" height="849" alt="image" src="https://github.com/user-attachments/assets/88c680a8-7346-44cb-8519-4f51830d261b" />

