## C++에서 UMG Widget 활용 방법

#### 위젯의 주요한 메서드
- Constructor
    - 위젯이 생성될 때 가장 먼저 호출됩니다.
    - 초기화 코드를 여기에 작성합니다.
    - 이 시점에서는 아직 다른 UI 요소나 게임 로직에 액세스할 수 없습니다.
- NativePreConstruct
    - 위젯이 화면에 그려지기 전에 호출됩니다.
    - 위젯의 속성이나 자식 요소를 동적으로 조정하는 데 사용됩니다.
- NativeConstruct
    - 위젯이 화면에 완전히 그려진 후에 호출됩니다.
    - 위젯과 관련된 초기화 작업이나 이벤트 바인딩을 수행합니다.
    - 이 시점에서는 모든 UI 요소에 접근할 수 있으며, 위젯이 완전히 준비된 상태입니다.
- NativeDestruct
    - 위젯이 화면에서 제거될 때 호출됩니다.
    - 리소스 해제, 이벤트 언바인딩, 기타 청소 작업을 수행합니다.

#### Create Widget
- UWidget
    ``` c++
    USpacer* spacer = NewObject<USpacer>(this);
    spacer->SetSize(FVector2D(20, 0));
    BuildItemBox->AddChild(spacer);
    ```

- UUserWidget
    ``` c++
    TSubclassOf<UUserWidget> userCardWidget; // UClass에 대한 취득이 필요합니다.
    // userWidgetCard : UUserCardWidget class
    UUserCardWidget* card = CreateWidget<UUserCardWidget>(this, userCardWidget.get());
    if(card)
    {
        // 여기에서 card에 대한 작업을 할 수 있습니다.
    }
    ```

#### **주의사항**
- widget의 NativeConstructor의 호출타이밍 및 UI 설정
    ```c++
    // UserCardWidget.h    
    UCLASS()
    class PROJ_NAME_API UUserCardWidget : public UUserWidget
    {
	    GENERATED_BODY()

    protected:
        UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
        UImage* BackgroundImage;
	    virtual void NativeConstruct() override;

    public:
        void SetImage(UTexture2D* texture);

    };
    ```
    ```c++
    // UserCardWidget.cpp    
    void UUserCardWidget::NativeConstruct()
    {
        // Image를 none으로 초기화합니다.
        ResourceImage->SetBrushFromTexture(nullptr);
    }

    void UUserCardWidget::SetImage(UTexture2D* texture)
    {
        // 외부에서 Texture를 입력받아서 Image에 설정합니다.
		ResourceImage->SetBrushFromTexture(texture);
    }
    ```
    ```c++
    // UserCardWidget 활용
    TSubclassOf<UUserWidget> userCardWidget; // UClass에 대한 취득이 필요합니다.
    UTexture2D* texture;    // texture에 대한 취득이 필요합니다.
    // userWidgetCard : UUserCardWidget class
    UUserCardWidget* card = CreateWidget<UUserCardWidget>(this, userCardWidget.get());
    if(card)
    {
        card->SetImage(texture);
    }
    ```
    - 위의 경우 UUserCardWidget::NativeConstruct, UUserCardWidget::SetImage의 호출 순위가 NativeConstruct -> SetImage가 아닐 수 있습니다.
    - NativeConstruct는 화면에 렌더링된 후에 호출됩니다.
    - 멤버변수의 초기화는 Constructor에서 설정합니다.
    - UI 요소에 대한 설정은 NativeConstruct에서 설정합니다.