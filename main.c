#include <windows.h>
#include "resource.h"
#include "Rain.h"
#include<time.h>

///////////////////我自己添加的东西//////////////////
/*声明一个雨天气系统*/
Rain g_rainSystem;
///////////////////////////////////////////////////// 

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/* Current app instance */
HINSTANCE hInst;
/*  Make the class name into a global variable  */
TCHAR szClassName[] = TEXT("WindowsApp");

int WINAPI
WinMain (HINSTANCE hThisInstance,
         HINSTANCE hPrevInstance,
         LPSTR lpszArgument,
         int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

		/* Save this instance */
		hInst = hThisInstance;
	
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE (IDC_RAINFALL);
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           TEXT("池塘夜降彩色雨"), 		/* Title Text */
           WS_CAPTION|WS_SYSMENU, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           0,       						/* where the window ends up on the screen */
           800,       /* The programs width */
           600,                 	/* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);
    UpdateWindow(hwnd);
	
	//为了让渲染时序得以控制 
	double lastTime = clock();
	double currentTime,timeDelta;
	
    while(messages.message != WM_QUIT)
    {
		if(PeekMessage(&messages,NULL,0,0,PM_REMOVE))
		{
	        /* Translate virtual-key messages into character messages */
	        TranslateMessage(&messages);
	        /* Send message to WindowProcedure */
	        DispatchMessage(&messages);
    	}
    	else
    	{
			//这里添加渲染的东西 
			
			//除了Rain_Render，其他都是控制渲染时序的辅助代码 
			currentTime = clock();
			timeDelta = (currentTime - lastTime)*0.001f;
			
			Rain_Render(&g_rainSystem,timeDelta);
			
			lastTime = currentTime;
		}
	}

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK
WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	TCHAR szHello[] = TEXT("Hello, C-Free!");
	
	//雨系统的消息处理 
	Rain_MessageHandler(&g_rainSystem,message,wParam,lParam);
	
    switch (message)                  /* handle the messages */
    {
    	case WM_CREATE:
    	{
			Rain_InitSystem(&g_rainSystem,hwnd);
			break;
		}
		case WM_SIZE:
		{
			RECT rect;
			GetClientRect(hwnd,&rect);
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			Rain_SetWidth(&g_rainSystem,width);
			Rain_SetHeight(&g_rainSystem,height);
			g_rainSystem.m_landLevelEnd = height;
			
			break;
		}
		case WM_COMMAND:
    		switch (LOWORD(wParam))
    		{
		    case IDM_ABOUT:
				MessageBox (hwnd, TEXT ("程序名称：池塘夜降彩色雨\n\
				\n版本： v1.0\n完成日期：2013/06/09\n\n最终解释权归廖高灿所有\n\
				\n抄袭必究！"),
                        TEXT ("About"), MB_OK | MB_ICONINFORMATION);
				break;
			case IDM_EXIT:
				DestroyWindow(hwnd);
				break;
			default:
				return DefWindowProc(hwnd, message, wParam, lParam);	    		
		    }
    		break;
    	case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			/* TODO: Add any drawing code here... */
			
			//GetClientRect(hwnd, &rt);
			//DrawText(hdc, szHello, lstrlen(szHello), &rt, DT_CENTER|DT_VCENTER|DT_SINGLELINE);	
			EndPaint(hwnd, &ps);
			break;
        case WM_DESTROY:
        	/*退出程序前要销毁雨天气系统*/
        	Rain_DestroySystem(&g_rainSystem);
        	
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
