from transformers import BertTokenizer, BertModel

model_path = '/home/zhangbo/model/AI-ModelScope/bert-base-uncased'
# 加载预训练的BERT模型和分词器
tokenizer = BertTokenizer.from_pretrained(model_path)
model = BertModel.from_pretrained(model_path)

# 新增词汇
new_tokens = ['new_word1', 'new_word2', 'new_word3','你','好']

# 更新分词器
tokenizer.add_tokens(new_tokens)

# 扩展模型的词汇表
model.resize_token_embeddings(len(tokenizer))

# 测试新的分词器
text = "This is a new_word1 example. new_word 你好"

# 对文本进行编码
encoded_input = tokenizer(text, return_tensors='pt')

# 通过模型进行推理
output = model(**encoded_input)

# 获取输入的Token ID
input_ids = encoded_input['input_ids'][0]

# 将Token ID还原为Token
decoded_tokens = tokenizer.convert_ids_to_tokens(input_ids)

# 输出结果
print("Original text:", text)
print("Encoded tokens:", decoded_tokens)

# 打印模型输出的形状
print("Output shape:", output.last_hidden_state.shape)
