*DFAʹ��˵��**/
 
1��������ʽ˵����
 Ԫ����˵����
    [] 0����1��
    <> 1������
    {} 0������
    $* ���� ��ע�����������Ҫ�пո�
    () ��������
    |  ��
    \ Ϊת���ַ�(��Ҫת����� �ո�,tab,$,-,(,),{},<>,[],|,@)
    //Ϊע����
 ���һ��Ϊ�����У���������($email:100|$ipadd:101)��ʽ����ʾʶ��$email��$ipadd,����ֵ�ֱ���100��101
 �����в��ܰ����Ǳ���������:\d+Ϊ����ֵ
  
  ����μ�default.txt�ĸ�ʽ
 
2,����ʹ��˵��(linux�汾)
  (/home/zhangbo/finitemachine/bin)
  1,./dfabuild  <�����ļ�_in> <�Զ����ļ�_out>
  
  2,./find  <�Զ����ļ�_in> <�ı��ļ�_multi_in> <����ļ�>
 
3,Դ��ʹ��

CNFABuilder���ṩ
(/home/zhangbo/finitemachine/dfabuilder)
 
  //�����Զ����ļ�����
 ������Դ�ļ���/home/zhangbo/finitemachine/bin/default.txt
  
  LoadRuleFile(...); //��������ļ�,����ʼ��NFA
  
  CreateNFA2DFA(...);//��NFAת����DFA
  
  SaveDFAtoFile(...);//DFA��ɶ������ļ����洢��ʽΪ��ͷ��㡿��-1����..�Զ����ṹ�塿
  
  Release();//�ͷſռ�
  
  ����ʹ�ô���μ�/home/zhangbo/finitemachine/dfabuilder/dfabuild.cpp
  
  //���ҽӿ��ļ�
  (/home/zhangbo/finitemachine/demo)
   ������Դ�ļ���/home/zhangbo/finitemachine/bin/mydfa.net

  CSegRule * NFAB=CreateSegRuleInstance()//�����Զ���ʵ��
  
  int len=NFAB->Search(&nBuf[j],nCovlen-j,&retnode);//���Ҳ����ؽ�������Լ�����ṹ��
  
  Release(NFAB);//�ռ��ͷ�
  
  ����ʹ�ô���μ�/home/zhangbo/finitemachine/demo/find.cpp
  
4,����Ч��
  80M��������,�������е�����,Ӣ��,�绰,����,ip��ַ,�ո�,�����2761378�������ƽ����ʱ7s
  ����2G�ڴ�,�����ٶ�ԼΪ 10M/s