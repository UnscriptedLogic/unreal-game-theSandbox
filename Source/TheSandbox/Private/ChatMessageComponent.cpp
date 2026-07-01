#include "ChatMessageComponent.h"

#include "GameFramework/Controller.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

UChatMessageComponent::UChatMessageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UChatMessageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UChatMessageComponent, ChatHistory);
}

void UChatMessageComponent::SubmitChatMessage(const FString& Message, FName Channel)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (UChatMessageComponent* GameStateChat = GetGameStateChatComponent())
		{
			GameStateChat->AddChatMessage(GetOwnerPlayerName(), Message, Channel, EChatMessageType::Player);
			return;
		}

		AddChatMessage(GetOwnerPlayerName(), Message, Channel, EChatMessageType::Player);
		return;
	}

	if (Cast<AGameStateBase>(GetOwner()))
	{
		if (UChatMessageComponent* PlayerControllerChat = GetLocalPlayerControllerChatComponent())
		{
			PlayerControllerChat->SubmitChatMessage(Message, Channel);
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("ChatMessageComponent on GameState needs a ChatMessageComponent on the local PlayerController to submit client messages."));
		return;
	}

	ServerSubmitChatMessage(Message, Channel);
}

void UChatMessageComponent::AddPlayerChatMessage(APlayerState* SenderPlayerState, const FString& Message, FName Channel)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	AddChatMessage(SenderPlayerState ? SenderPlayerState->GetPlayerName() : TEXT("Player"), Message, Channel, EChatMessageType::Player);
}

void UChatMessageComponent::SendServerMessage(const FString& Message, FName Channel)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	AddChatMessage(TEXT("Server"), Message, Channel, EChatMessageType::Server);
}

TArray<FReplicatedChatMessage> UChatMessageComponent::GetChatHistory() const
{
	return ChatHistory;
}

void UChatMessageComponent::ClearChatHistory()
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	ChatHistory.Reset();
	LastBroadcastMessageId = 0;
	OnChatHistoryChanged.Broadcast(ChatHistory);
}

void UChatMessageComponent::OnRep_ChatHistory()
{
	BroadcastNewMessages();
	OnChatHistoryChanged.Broadcast(ChatHistory);
}

void UChatMessageComponent::ServerSubmitChatMessage_Implementation(const FString& Message, FName Channel)
{
	if (UChatMessageComponent* GameStateChat = GetGameStateChatComponent())
	{
		GameStateChat->AddChatMessage(GetOwnerPlayerName(), Message, Channel, EChatMessageType::Player);
		return;
	}

	AddChatMessage(GetOwnerPlayerName(), Message, Channel, EChatMessageType::Player);
}

void UChatMessageComponent::AddChatMessage(const FString& Sender, const FString& Message, FName Channel, EChatMessageType Type)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	const FString CleanMessage = SanitizeMessage(Message);
	if (CleanMessage.IsEmpty())
	{
		return;
	}

	FReplicatedChatMessage NewMessage;
	NewMessage.MessageId = NextMessageId++;
	NewMessage.Sender = Sender.IsEmpty() ? TEXT("Player") : Sender;
	NewMessage.Message = CleanMessage;
	NewMessage.Channel = Channel.IsNone() ? FName(TEXT("Global")) : Channel;
	NewMessage.Type = Type;
	NewMessage.ServerTimeSeconds = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;

	ChatHistory.Add(NewMessage);

	while (ChatHistory.Num() > MaxHistoryMessages)
	{
		ChatHistory.RemoveAt(0);
	}

	BroadcastNewMessages();
	OnChatHistoryChanged.Broadcast(ChatHistory);
}

void UChatMessageComponent::BroadcastNewMessages()
{
	for (const FReplicatedChatMessage& ChatMessage : ChatHistory)
	{
		if (ChatMessage.MessageId > LastBroadcastMessageId)
		{
			OnChatMessageReceived.Broadcast(ChatMessage);
			LastBroadcastMessageId = ChatMessage.MessageId;
		}
	}
}

UChatMessageComponent* UChatMessageComponent::GetGameStateChatComponent() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	AGameStateBase* GameState = World->GetGameState();
	return GameState ? GameState->FindComponentByClass<UChatMessageComponent>() : nullptr;
}

UChatMessageComponent* UChatMessageComponent::GetLocalPlayerControllerChatComponent() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	return PlayerController ? PlayerController->FindComponentByClass<UChatMessageComponent>() : nullptr;
}

FString UChatMessageComponent::SanitizeMessage(const FString& Message) const
{
	FString CleanMessage = Message;
	CleanMessage.TrimStartAndEndInline();
	CleanMessage.ReplaceInline(TEXT("\r"), TEXT(" "));
	CleanMessage.ReplaceInline(TEXT("\n"), TEXT(" "));

	if (CleanMessage.Len() > MaxMessageLength)
	{
		CleanMessage.LeftInline(MaxMessageLength);
		CleanMessage.TrimEndInline();
	}

	return CleanMessage;
}

FString UChatMessageComponent::GetOwnerPlayerName() const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return TEXT("Player");
	}

	if (const APlayerState* PlayerState = Cast<APlayerState>(Owner))
	{
		return PlayerState->GetPlayerName();
	}

	if (const APawn* Pawn = Cast<APawn>(Owner))
	{
		if (const APlayerState* PlayerState = Pawn->GetPlayerState())
		{
			return PlayerState->GetPlayerName();
		}
	}

	if (const AController* Controller = Cast<AController>(Owner))
	{
		if (const APlayerState* PlayerState = Controller->GetPlayerState<APlayerState>())
		{
			return PlayerState->GetPlayerName();
		}
	}

	return Owner->GetName();
}
