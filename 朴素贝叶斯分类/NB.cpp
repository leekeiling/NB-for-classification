#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <cmath>
#include <math.h>
#include <queue>
using namespace std;

const int JOY = 1;
const int FEAR = 0;
const int SAD = 2;
const int SURPRISE = 3;
const int ANGER = 4;
const int DISGUST = 5;

string not_relate[71]={//����ش�����Լ��ٷ����Ԫ�ص�Ӱ�� 
	"on","to","with","a","will","in","of","x","re","s",
	"are","on","by","could","was","at","the","is","as","its",
	"from","too","near","we","our","an","did","after","their","while",
	"would","who","and","may","us","u","over","be","can",
	"your","if","two","three","you","one","it","t","ever","i",
	"ps","before","or","than","under","here","out","off","n","me",
	"al","th","what","been","all","m","now","his","pm","st","says"};
	
//�ж��Ƿ��ڲ���������� 
bool InNotRelate(string s){
	for(int i=0;i<70;i++)
		if(s==not_relate[i]) return true;
	return false;
}

vector <string> letterSet;
double p[6][3000]={0.0};//��Ȼ�Ⱦ��� 
double pEmotion[6];//������� 

vector<string> split2(string str,string pattern)
{
    int pos;
    vector<string> result;
    str+=pattern;//��չ�ַ����Է������
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

bool ifIn(vector<string> v,string s) {
	for(int i=0;i<v.size();i++) {
		if(v[i]==s) return true;
	}	
	return false;
}

void getProbability(){
	int cntEmotion[6]={0};//��¼ÿ����������Ŀ 
	int cntWordEmotion[6]={0}; //��¼����ÿ���������ʵ�����Ŀ�� 
	int txtnum = 0;
	vector<string> txtVector[1000];//ÿƪ���µĵ��ʣ� 
	fstream f("D:\\train_set.csv");
	int* emotion = new int[2000];
	string s;
	while(getline(f,s)){
		vector<string> temVector = split2(s,",");
		txtVector[txtnum] = split2(temVector[0]," ");
		string em = temVector[1];
		if(em=="joy"){
			emotion[txtnum]=JOY;
			cntEmotion[JOY]++;
			cntWordEmotion[JOY]+=txtVector[txtnum].size();
		}
		else if(em=="sad") {
			emotion[txtnum]=SAD;
			cntEmotion[SAD]++;
			cntWordEmotion[SAD]+=txtVector[txtnum].size();
		}
		else if(em=="anger"){
			emotion[txtnum]=ANGER;
			cntEmotion[ANGER]++;
			cntWordEmotion[ANGER]+=txtVector[txtnum].size();
		}
		else if(em=="surprise")	{
			emotion[txtnum]=SURPRISE;
			cntEmotion[SURPRISE]++;
			cntWordEmotion[SURPRISE]+=txtVector[txtnum].size();
		}
		else if(em=="fear")	{
			emotion[txtnum]=FEAR;
			cntEmotion[FEAR]++;
			cntWordEmotion[FEAR]+=txtVector[txtnum].size();
		}
		else if(em=="disgust") {
			emotion[txtnum]=DISGUST;
			cntEmotion[DISGUST]++;
			cntWordEmotion[DISGUST]+=txtVector[txtnum].size();
		}
		for(int i=0;i<txtVector[txtnum].size();i++){
			if(!ifIn(letterSet,txtVector[txtnum][i]) && !InNotRelate(txtVector[txtnum][i])) 
				letterSet.push_back(txtVector[txtnum][i]);
		}
		txtnum++;
	}	
	for(int i=0;i<txtnum;i++){//����������Ե���Ȼ�� 
		for(int j=0;j<letterSet.size();j++){
			for(int k=0;k<txtVector[i].size();k++){
				if(letterSet[j]==txtVector[i][k]){
					p[emotion[i]][j] = p[emotion[i]][j] + 1;
				}
			}
		}
	}
	for(int i=0;i<6;i++) //lidstoneƽ��������Ȼ�� 
		for(int j=0;j<letterSet.size();j++){
			p[i][j] = (p[i][j]+0.7)/(cntWordEmotion[i]*1.0+letterSet.size()*0.7);
		}
	for(int i=0;i<6;i++) //lidstoneƽ�������������
		 pEmotion[i] = (cntEmotion[i]*1.0+0.5)/(txtnum*1.0+6.0*0.5);
	for(int i=0;i<6;i++)	cout<<pEmotion[i]<<" ";
	cout<<endl;
}


int main(){
	getProbability();
	fstream f("D:\\nb_validation_set.csv");
	fstream result("D:\\result_for_NB.csv",ios::out);
	string s;
	int id=1;
	result<<"textid"<<","<<"label"<<"\n";
	while(getline(f,s)){
		vector<string> temVector = split2(s,",");
		vector<string> test = split2(temVector[0]," ");
		double later_p[6];
		for(int i=0;i<6;i++) later_p[i]=1.0;
		for(int i=0;i<letterSet.size();i++){
			for(int j=0;j<test.size();j++){
				if(letterSet[i]==test[j]){
					for(int k=0;k<6;k++){
						later_p[k]*=p[k][i];//���������� 
					}
				}
			}
		}
		int pos;
		double MAX=-1;
		for(int i=0;i<6;i++){
			//cout<<later_p[i]<<" ";
			//cout<<pEmotion[i]<<endl;
			later_p[i]*=pEmotion[i]; //���������� 
			//cout<<later_p[i]<<" ";
			if(later_p[i]>MAX){
				MAX = later_p[i];
				pos = i;
			}
		}
		switch(pos){
			case JOY:
				result<<id<<","<<"joy"<<"\n";
				break;
			case SAD:
				result<<id<<","<<"sad"<<"\n";
				break;
			case FEAR:
				result<<id<<","<<"fear"<<"\n";
				break;
			case DISGUST:
				result<<id<<","<<"disgust"<<"\n";
				break;
			case SURPRISE:
				result<<id<<","<<"surprise"<<"\n";
				break;
			case ANGER:
				result<<id<<","<<"anger"<<"\n";
				break;
		}
		id++;
	}
} 
