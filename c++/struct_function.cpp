#include <iostream>
#include <cstring>

using namespace std;

// ��typedef��ʽ����һ���ṹ������ Books
typedef struct Books
{
    char title[50];
    char author[50];
    char subject[100];
    int book_id;
    Books()
    {
        book_id = 2;
    }
} Books;

int main()
{
    // ���Խṹ���ڶ��庯��
    Books book1=Books();
    
    cout<<book1.book_id;
    //    Books Book1;        // ����ṹ������ Books �ı��� Book1
    //    Books Book2;        // ����ṹ������ Books �ı��� Book2

    //    // Book1 ����
    //    strcpy( Book1.title, "C++ �̳�");
    //    strcpy( Book1.author, "Runoob");
    //    strcpy( Book1.subject, "�������");
    //    Book1.book_id = 12345;

    //    // Book2 ����
    //    strcpy( Book2.title, "CSS �̳�");
    //    strcpy( Book2.author, "Runoob");
    //    strcpy( Book2.subject, "ǰ�˼���");
    //    Book2.book_id = 12346;

    //    // ��� Book1 ��Ϣ
    //    cout << "��һ������� : " << Book1.title <<endl;
    //    cout << "��һ�������� : " << Book1.author <<endl;
    //    cout << "��һ������Ŀ : " << Book1.subject <<endl;
    //    cout << "��һ���� ID : " << Book1.book_id <<endl;

    //    // ��� Book2 ��Ϣ
    //    cout << "�ڶ�������� : " << Book2.title <<endl;
    //    cout << "�ڶ��������� : " << Book2.author <<endl;
    //    cout << "�ڶ�������Ŀ : " << Book2.subject <<endl;
    //    cout << "�ڶ����� ID : " << Book2.book_id <<endl;

    return 0;
}