// Copyright by Nodrop
#include <bits/stdc++.h>
using namespace std;

#define sz(v)               (ll)v.size()

struct Token {
    char first;
    char second;
};

typedef int ll;
typedef vector<ll> vl;
typedef vector<vl> vvl;
typedef pair<ll,ll> pl;
typedef vector<Token> vt;
typedef vector<vt> vvt;
typedef vector<bool> vb;

struct Coordinate{
    ll x;
    ll y;
};

struct Node {
    Coordinate pos;
    ll level;
};

ll buffer_size,matrix_width,matrix_height,num_sequences,max_reward;

vvt matrix,sequence;
vl sequence_reward;
vt tempVec;
Token tempToken;
ll tempVal;
Coordinate tempCoord;
ll countAllSolution;
ll numInput;

vector<Coordinate> candidateSolution;
vector<Coordinate> realSolution;
vector<vb> isVisited;
double time_taken;
char isRecursive;

char convertIntToChar(ll a){
    if(a>=1 && a<=10) return (char)(a+47);
    else return (char)(a+54); // Otherwise, 11-36
}

void randomizeInputMat(ll row, ll col){
    for(ll i = 0; i<row; i++){
        for(ll j = 0; j<col; j++){
            tempToken.first = convertIntToChar(rand()%37);
            tempToken.second = convertIntToChar(rand()%37);
            tempVec.emplace_back(tempToken);
        }
        matrix.emplace_back(tempVec);
        tempVec.clear();
    }
}

void randomizeSequence(ll num){
    for(ll i = 0; i<num; i++){
        ll length = rand()%(buffer_size) + 1;
        ll currX = rand()%(matrix_width);
        ll currY = rand()%(matrix_height);
        tempToken.first = matrix[currY][currX].first;
        tempToken.second = matrix[currY][currX].second;
        isVisited[currY][currX] = true;
        tempVec.emplace_back(tempToken);
        ll choseFirstDir = rand()%2; // 1 for vertical, 0 for horizontal
        for(ll j = 0; j<length-1; j++){
            if(choseFirstDir){
                ll tempRandY = rand()%(matrix_height);
                while(currY==tempRandY||isVisited[tempRandY][currX]){
                    tempRandY = rand()%(matrix_height);
                }
                currY=tempRandY;
            }else{
                ll tempRandX = rand()%(matrix_width);
                while(currX==tempRandX||isVisited[currY][tempRandX]){
                    tempRandX = rand()%(matrix_width);
                }
                currX=tempRandX;
            }
            tempToken.first = matrix[currY][currX].first;
            tempToken.second = matrix[currY][currX].second;
            isVisited[currY][currX] = true;
            tempVec.emplace_back(tempToken);
            choseFirstDir++;
            choseFirstDir %= 2;
        }
        sequence.emplace_back(tempVec);
        sequence_reward.emplace_back((rand()%203) - 101);
        tempVec.clear();
        for(ll k = 0; k<matrix_height; k++){
            for(ll l = 0; l<matrix_width; l++){
                isVisited[k][l] = false;
            }
        }
    }
}

void initVisitChecker(){
    for(ll i = 0; i<matrix_height; i++){
        vb tempVb;
        for(ll j = 0; j<matrix_width; j++){
            tempVb.push_back(false);
        }
        isVisited.emplace_back(tempVb);
    }
}

void printInput(){
    cout << buffer_size << "\n";
    cout << matrix_width << " " << matrix_height << "\n";
    for(ll i=0; i<matrix_height; i++){
        for(ll j=0; j<matrix_width; j++){
            cout << matrix[i][j].first << matrix[i][j].second << " ";
        }
        cout << "\n";
    }

    cout << num_sequences << "\n";

    for(ll i = 0; i<num_sequences; i++){
        for(ll j = 0; j<sz(sequence[i]); j++){
            cout << sequence[i][j].first << sequence[i][j].second << " ";
        }
        cout << "\n";
        cout << sequence_reward[i] << "\n";
    }
}

void saveInput(string namaFile){
    ofstream outputFile("../test/input/"+namaFile);
    if (outputFile.is_open()) {
        outputFile << buffer_size << "\n";
        outputFile << matrix_width << " " << matrix_height << "\n";
        for(ll i=0; i<matrix_height; i++){
            for(ll j=0; j<matrix_width; j++){
                outputFile << matrix[i][j].first << matrix[i][j].second << " ";
            }
            outputFile << "\n";
        }

        outputFile << num_sequences << "\n";

        for(ll i = 0; i<num_sequences; i++){
            for(ll j = 0; j<sz(sequence[i]); j++){
                outputFile << sequence[i][j].first << sequence[i][j].second << " ";
            }
            outputFile << "\n";
            outputFile << sequence_reward[i] << "\n";
        }
        outputFile.close();
        cout << "Data has been saved successfully.\n";
    } else {
        cout << "Unable to open file.\n";
    }
}

void manualInputDataTxtFile(string namaFile){
    ifstream inputFile("../test/input/"+namaFile);

    while(!inputFile.is_open()){
        cout << "File yang dicari tidak ada!\n";
        cout << "Masukkan nama file yang ingin diinput dengan format .txt: ";
        cin >> namaFile;
        inputFile.open("../test/input/"+namaFile);
        cout << "\n";
    }

    inputFile >> buffer_size >> matrix_width >> matrix_height;
    for(ll i=0; i<matrix_height; i++){
        for(ll j=0; j<matrix_width; j++){
            inputFile >> tempToken.first >> tempToken.second;
            tempVec.emplace_back(tempToken);
        }
        matrix.emplace_back(tempVec);
        tempVec.clear();
    }
    inputFile >> num_sequences;
    inputFile.ignore();
    for(ll i = 0; i<num_sequences; i++){
        string tempSeq; getline(inputFile,tempSeq);
        ll szTempSeq = sz(tempSeq);
        for(ll j = 0; j<szTempSeq; j++){
            int tempMod = j%3;
            if(tempMod==0){
                tempToken.first = tempSeq[j];
            }else if(tempMod==1){
                tempToken.second = tempSeq[j];
                tempVec.emplace_back(tempToken);
            }
        }
        sequence.emplace_back(tempVec);
        tempVec.clear();
        inputFile >> tempVal;
        sequence_reward.emplace_back(tempVal);
        inputFile.ignore();
    }
    initVisitChecker();
    inputFile.close();
}

void generateRandInput(){
    cout << "Enter buffer size: ";
    cin >> buffer_size;
    cout << "\n";
    cout << "Enter matrix width: ";
    cin >> matrix_width;
    cout << "\n";
    cout << "Emter matrix height: ";
    cin >> matrix_height;
    cout << "\n";
    cout << "Enter number of sequences: ";
    cin >> num_sequences;
    cout << "\n";
    
    srand(time(NULL));
    randomizeInputMat(matrix_height, matrix_width);
    initVisitChecker();
    randomizeSequence(num_sequences);

    cout << "Random Input Generated: \n";
    printInput();

    cout << "\n";
    cout << "Do you want to save the input? (Y/N): ";
    char isSimpanInput;
    cin >> isSimpanInput;
    cout << "\n";
    if (isSimpanInput == 'Y' || isSimpanInput == 'y') {
        string namaFile;
        cout << "Enter File Name with .txt extension: ";
        cin >> namaFile;
        saveInput(namaFile);
    }
}

void inputData(){
    cout << "Silahkan pilih model input data anda: \n";
    cout << "1. Input Data Manual\n";
    cout << "2. Input Data dengan Random Generator\n";
    cout << "Command [1,2]: ";
    cin >> numInput;
    cout << "\n";
    if(numInput==1){
        cout << "Masukkan nama file yang ingin diinput dengan format .txt: ";
        string namaFile; cin >> namaFile;
        manualInputDataTxtFile(namaFile);
    }else{
        generateRandInput();
    }
}

void checkSequences(){
    ll szCandidate = sz(candidateSolution);
    for(ll max_lengthCandidate = 1; max_lengthCandidate<=szCandidate; max_lengthCandidate++){
        ll countReward = 0;
        for(ll i = 0; i<num_sequences; i++){
            if(sequence_reward[i]<=0) continue; // Kalo value reward <= 0 langsung skip, percuma
            ll szSeq = sz(sequence[i]);
            if(max_lengthCandidate>=szSeq){
                for(ll j = 0; j<max_lengthCandidate; j++){
                    if(max_lengthCandidate-j>=szSeq){
                        bool flag = true;
                        for(ll k = 0; k<szSeq; k++){
                            if(sequence[i][k].first!=matrix[candidateSolution[j+k].y][candidateSolution[j+k].x].first
                            || sequence[i][k].second!=matrix[candidateSolution[j+k].y][candidateSolution[j+k].x].second
                            ){
                                flag = false;
                                break;
                            }
                        }
                        if(flag){
                            countReward += sequence_reward[i];
                            break;
                        }
                    }
                }
            }
        }
        if(countReward>max_reward){
            max_reward = countReward;
            realSolution.clear();
            for(ll i = 0; i<max_lengthCandidate; i++){
                realSolution.emplace_back(candidateSolution[i]);
            }
        }
    }
}

void searchBruteForce1(ll row,ll col){
    stack<Node> tempStack;
    Node tempPl1,tempPl2;
    tempPl1.pos.x = col; tempPl1.pos.y = row; tempPl1.level = 0;
    tempStack.push(tempPl1);
    ll currLevel = 0;
    while (!tempStack.empty())
    { 
        tempPl1 = tempStack.top();
        tempStack.pop();
        ll tempCandSz = sz(candidateSolution);
        if(tempPl1.level+1<=tempCandSz){
            for(ll i = 0; i<=(tempCandSz-tempPl1.level-1); i++){
                isVisited[candidateSolution.back().y][candidateSolution.back().x] = false;
                candidateSolution.pop_back();
            }
        }
        currLevel = tempPl1.level;
        tempCoord.x = tempPl1.pos.x;
        tempCoord.y = tempPl1.pos.y;
        isVisited[tempPl1.pos.y][tempPl1.pos.x] = true;
        candidateSolution.emplace_back(tempCoord);

        if(currLevel==buffer_size-1){
            countAllSolution++;
            checkSequences();
            continue;
        }

        ll tempMod = currLevel%2;
        ll length = (tempMod == 0) ? matrix_height : matrix_width;
        ll indexMove = (tempMod == 0) ? tempPl1.pos.y : tempPl1.pos.x;

        // Maju!
        for (ll i = length-1; i>=indexMove+1; i--){
            if(tempMod==0){
                if(isVisited[i][tempPl1.pos.x]) continue;
                tempPl2.pos.y = i; tempPl2.pos.x = tempPl1.pos.x;
            }else{
                if(isVisited[tempPl1.pos.y][i]) continue;
                tempPl2.pos.y = tempPl1.pos.y; tempPl2.pos.x = i;
            }
            tempPl2.level = currLevel+1;
            tempStack.push(tempPl2);
        }

        // Mundur
        for (ll i=0; i <=indexMove-1; i++){
            if(tempMod==0){
                if(isVisited[i][tempPl1.pos.x]) continue;
                tempPl2.pos.y = i; tempPl2.pos.x = tempPl1.pos.x;
            }else{
                if(isVisited[tempPl1.pos.y][i]) continue;
                tempPl2.pos.y = tempPl1.pos.y; tempPl2.pos.x = i;
            }
            tempPl2.level = currLevel+1;
            tempStack.push(tempPl2);
        }
    }
}

void searchBruteForce2Rec(ll level, ll row, ll col) {
    tempCoord.x = col;
    tempCoord.y = row;
    isVisited[row][col] = true;
    candidateSolution.emplace_back(tempCoord);

    if(level==buffer_size-1){
        countAllSolution++;
        checkSequences();
        return;
    }

    ll tempMod = level%2;
    ll length = (tempMod == 0) ? matrix_height : matrix_width;
    ll indexMove = (tempMod == 0) ? row : col;

    // Next, Mundur!
    for(ll i = indexMove-1; i>=0; i--){
        if(tempMod == 0){
            if(isVisited[i][col]) continue;
            searchBruteForce2Rec(level+1,i,col);
        }else {
            if(isVisited[row][i]) continue;
            searchBruteForce2Rec(level+1,row,i);
        }
        isVisited[candidateSolution.back().y][candidateSolution.back().x] = false;
        candidateSolution.pop_back();
    }

    // Next, Maju!
    for(ll i = indexMove+1; i<length; i++){
        if(tempMod == 0){
            if(isVisited[i][col]) continue;
            searchBruteForce2Rec(level+1,i,col);
        }else {
            if(isVisited[row][i]) continue;
            searchBruteForce2Rec(level+1,row,i);
        }
        isVisited[candidateSolution.back().y][candidateSolution.back().x] = false;
        candidateSolution.pop_back();
    }
}

void printSolution(){
    cout << "\n";
    cout << "Output:\n";
    cout << "Banyak candidate solusi: " << countAllSolution << "\n";
    cout << "Possible Solution: " << "\n";
    cout << max_reward << "\n";

    ll szSolution = sz(realSolution);
    if(szSolution>=1){
        for(ll i = 0; i<szSolution; i++){
            cout << matrix[realSolution[i].y][realSolution[i].x].first << matrix[realSolution[i].y][realSolution[i].x].second << " ";
        }
        cout << "\n";

        for(ll i = 0; i<szSolution; i++){
            cout << realSolution[i].x + 1 << ", " << realSolution[i].y + 1 << "\n";
        }
        cout << "\n";
    }else{
        cout << "No Solution\n";
    }
    cout << "Time Execution: " << fixed << time_taken * 1000 << setprecision(5); 
    cout << " ms\n";
}

void printSolutionToTXT(string namaFile){
    ofstream outputFile("../test/output/"+namaFile);
    if (outputFile.is_open()) {
        outputFile << max_reward << "\n";
        ll szSolution = sz(realSolution);
        if(szSolution>=1){
            for(ll i = 0; i<szSolution; i++){
                outputFile << matrix[realSolution[i].y][realSolution[i].x].first << matrix[realSolution[i].y][realSolution[i].x].second << " ";
            }
            outputFile << "\n";

            for(ll i = 0; i<szSolution; i++){
                outputFile << realSolution[i].x + 1 << ", " << realSolution[i].y + 1 << "\n";
            }
            outputFile << "\n";
        }else{
            outputFile << "No Solution\n";
        }
        outputFile << "Time Execution: " << fixed << time_taken * 1000 << setprecision(5); 
        outputFile << " ms\n";
        outputFile.close();
        cout << "Data has been saved successfully.\n";
    } else {
        cout << "Unable to open file.\n";
    }
}

// Solve with brute force
void solve(){
    for(ll i = 0; i<matrix_width; i++){
        if(isRecursive=='Y'){
            searchBruteForce2Rec(0,0,i);
        }
        else{
            searchBruteForce1(0,i);
        }
        while(!candidateSolution.empty()){
            isVisited[candidateSolution.back().y][candidateSolution.back().x] = false;
            candidateSolution.pop_back();
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    cout << "Apakah ingin menggunakan algo recursive (Y\\N)? ";
    cin >> isRecursive;
    cout << "\n";
    inputData();
    clock_t start, end;
    start = clock();
    solve();
    end = clock();
    time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    printSolution();
    cout << "\n";
    cout << "Apakah ingin menyimpan solusi? [Y/N]: ";
    char isSimpanSolusi; cin >> isSimpanSolusi;
    cout << "\n";
    if(isSimpanSolusi=='Y'||isSimpanSolusi=='y'){
        cout << "Masukkan Nama File dengan format .txt: ";
        string namaFile; cin >> namaFile;
        printSolutionToTXT(namaFile);
    }
    cout << "Terima kasih telah bermain :D\n";
    return 0;
}