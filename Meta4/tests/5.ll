declare i32 @putchar(i32)
declare i32 @getchar()
define i32 @main() {
call void @_GLOBAL_DECLARATIONS()
%1 = alloca i32
store i32 3, i32* %1
%2 = alloca i32
store i32 3, i32* %2
br label %3 ********

%4= load i32, i32* %1
%5 = icmp ne i32 %4, 0 
br i1 %5, label %6, label %8 ****

ret i32 -1
br label %3 ********

%9= load i32, i32* %1
%10 = icmp eq i32 %9, 2
br i1 %10, label %11, label %12

br label %13

br label %13

%14= load i32, i32* %1
%15 = icmp eq i32 %14, 2
br i1 %15, label %16, label %19

%17= load i32, i32* %1
ret i32 %17
br label %21

ret i32 5
br label %21

br label %22

%23= load i32, i32* %2
%24 = icmp eq i32 %23, 3
br i1 %24, label %25, label %72

%26= load i32, i32* %1
%27 = icmp eq i32 %26, 2
br i1 %27, label %28, label %31

%29= load i32, i32* %1
ret i32 %29
br label %32

br label %32

%33= load i32, i32* %1
%34 = icmp eq i32 %33, 2
br i1 %34, label %35, label %38

%36= load i32, i32* %1
ret i32 %36
br label %39

br label %39

%40= load i32, i32* %1
%41 = icmp eq i32 %40, 2
br i1 %41, label %42, label %45

%43= load i32, i32* %1
ret i32 %43
br label %47

ret i32 5
br label %47

%48= load i32, i32* %1
%49 = icmp eq i32 %48, 2
br i1 %49, label %50, label %53

%51= load i32, i32* %1
ret i32 %51
br label %54

br label %54

%55= load i32, i32* %1
%56 = icmp eq i32 %55, 2
br i1 %56, label %57, label %60

%58= load i32, i32* %1
ret i32 %58
br label %62

ret i32 5
br label %62

%63= load i32, i32* %1
%64 = icmp eq i32 %63, 2
br i1 %64, label %65, label %68

%66= load i32, i32* %1
ret i32 %66
br label %70

ret i32 5
br label %70

ret i32 90
br label %22

ret i32 0
}
define void @_GLOBAL_DECLARATIONS(){
ret void
}
