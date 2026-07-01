#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChatMessageComponent.generated.h"

UENUM(BlueprintType)
enum class EChatMessageType : uint8
{
	Player,
	Server,
	System
};

USTRUCT(BlueprintType)
struct FReplicatedChatMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Chat")
	int32 MessageId = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Chat")
	FString Sender;

	UPROPERTY(BlueprintReadOnly, Category = "Chat")
	FString Message;

	UPROPERTY(BlueprintReadOnly, Category = "Chat")
	FName Channel = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = "Chat")
	EChatMessageType Type = EChatMessageType::Player;

	UPROPERTY(BlueprintReadOnly, Category = "Chat")
	float ServerTimeSeconds = 0.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatMessageReceived, const FReplicatedChatMessage&, ChatMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatHistoryChanged, const TArray<FReplicatedChatMessage>&, ChatHistory);

UCLASS(ClassGroup=(Multiplayer), meta=(BlueprintSpawnableComponent))
class THESANDBOX_API UChatMessageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UChatMessageComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category = "Chat")
	FOnChatMessageReceived OnChatMessageReceived;

	UPROPERTY(BlueprintAssignable, Category = "Chat")
	FOnChatHistoryChanged OnChatHistoryChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chat", meta=(ClampMin="1", ClampMax="500"))
	int32 MaxHistoryMessages = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chat", meta=(ClampMin="1", ClampMax="512"))
	int32 MaxMessageLength = 256;

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void SubmitChatMessage(const FString& Message, FName Channel = "Global");

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Chat")
	void AddPlayerChatMessage(APlayerState* SenderPlayerState, const FString& Message, FName Channel = "Global");

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Chat")
	void SendServerMessage(const FString& Message, FName Channel = "Global");

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Chat")
	TArray<FReplicatedChatMessage> GetChatHistory() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Chat")
	void ClearChatHistory();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_ChatHistory)
	TArray<FReplicatedChatMessage> ChatHistory;

	UFUNCTION()
	void OnRep_ChatHistory();

	UFUNCTION(Server, Reliable)
	void ServerSubmitChatMessage(const FString& Message, FName Channel);

private:
	UPROPERTY()
	int32 NextMessageId = 1;

	UPROPERTY(Transient)
	int32 LastBroadcastMessageId = 0;

	void AddChatMessage(const FString& Sender, const FString& Message, FName Channel, EChatMessageType Type);
	void BroadcastNewMessages();
	UChatMessageComponent* GetGameStateChatComponent() const;
	UChatMessageComponent* GetLocalPlayerControllerChatComponent() const;
	FString SanitizeMessage(const FString& Message) const;
	FString GetOwnerPlayerName() const;
};
